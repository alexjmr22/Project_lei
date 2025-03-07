import flask
import logging
import psycopg2
import time
import jwt
from datetime import datetime, timedelta
from functools import wraps
from flask import request

# add role in the code
# add responsable nurse in hospitalization
# put automatic squence in events_id's
# update bill in the triggers if hospitalization exists
# por variaveis id como sequencia

app = flask.Flask(__name__)

SECRET_KEY = 'candeeiro300721'  # Substitua por uma chave secreta segura

StatusCodes = {
    'success': 200,
    'api_error': 400,
    'internal_error': 500
}

##########################################################
## DATABASE ACCESS
##########################################################

def db_connection():
    db = psycopg2.connect(
        user='aulaspl',
        password='aulaspl',
        host='127.0.0.1',
        port='5432',
        database='projeto'
    )
    return db

##########################################################
## JWT FUNCTIONS
##########################################################

def create_token(user_id):
    payload = {
        'exp': datetime.utcnow() + timedelta(days=1000),
        'iat': datetime.utcnow(),
        'sub': user_id
    }
    return jwt.encode(payload, SECRET_KEY, algorithm='HS256')

def token_required(f):
    @wraps(f)
    def decorated(*args, **kwargs):
        token = None

        if 'Authorization' in flask.request.headers:
            token = flask.request.headers['Authorization'].split(" ")[1]

        if not token:
            return flask.jsonify({'status': StatusCodes['api_error'], 'errors': 'Token is missing!'})

        try:
            data = jwt.decode(token, SECRET_KEY, algorithms=['HS256'])
            current_user_id = data['sub']
        except jwt.ExpiredSignatureError:
            return flask.jsonify({'status': StatusCodes['api_error'], 'errors': 'Token has expired!'})
        except jwt.InvalidTokenError:
            return flask.jsonify({'status': StatusCodes['api_error'], 'errors': 'Invalid token!'})

        return f(current_user_id, *args, **kwargs)

    return decorated


##########################################################
## ENDPOINTS
##########################################################

@app.route('/')
def landing_page():
    return """
    Hello World (Python Native)!  <br/>
    <br/>
    Check the sources for instructions on how to use the endpoints!<br/>
    <br/>
    BD 2023-2024 Team<br/>
    <br/>
    """

# Register User
@app.route('/dbproj/register/<user_type>', methods=['POST'])
def register_user(user_type):
    logger.info('POST /dbproj/register/<user_type>')
    logger.debug(f'user_type: {user_type}')
    payload = flask.request.get_json()

    conn = db_connection()
    cur = conn.cursor()

    logger.debug(f'POST /dbproj/register - payload: {payload}')

    required_fields = ['username', 'password', 'contact']
    for field in required_fields:
        if field not in payload:
            response = {'status': StatusCodes['api_error'], 'errors': f'{field} value not in payload'}
            return flask.jsonify(response)
    
    try:
        cur.execute('INSERT INTO users (username, password, contact) VALUES (%s, %s, %s) RETURNING user_id',
                    (payload['username'], payload['password'], payload['contact']))
        user_id = cur.fetchone()[0]

        if user_type == 'patient':
            cur.execute('INSERT INTO patient (users_user_id) VALUES (%s)', (user_id,))
        elif user_type == 'doctor':
            cur.execute('INSERT INTO employee (users_user_id) VALUES (%s)', (user_id,))
            cur.execute('INSERT INTO doctor (medical_license, employee_users_user_id) VALUES (%s, %s)',
                        (payload['medical_license'], user_id))
        elif user_type == 'nurse':
            cur.execute('INSERT INTO employee (users_user_id) VALUES (%s)', (user_id,))
            cur.execute('INSERT INTO nurse (employee_users_user_id, category) VALUES (%s, %s)',
                        (user_id, payload['category']))
        elif user_type == 'assistant':
            cur.execute('INSERT INTO employee (users_user_id) VALUES (%s)', (user_id,))
            cur.execute('INSERT INTO assistant (employee_users_user_id) VALUES (%s)', (user_id,))
        else:
            return flask.jsonify({'status': StatusCodes['api_error'], 'errors': 'Invalid user type'})

        conn.commit()
        response = {'status': StatusCodes['success'], 'results (user_id)': user_id}

    except (Exception, psycopg2.DatabaseError) as error:
        logger.error(f'POST /dbproj/register/{user_type} - error: {error}')
        response = {'status': StatusCodes['internal_error'], 'errors': str(error)}
        conn.rollback()
    finally:
        if conn is not None:
            conn.close()

    return flask.jsonify(response)


# Authenticate user
@app.route('/dbproj/user', methods=['PUT'])
def login():
    logger.info('PUT /dbproj/user')
    payload = flask.request.get_json()
    username = payload['username']
    password = payload['password']

    if not username or not password:
        return flask.jsonify({'status': StatusCodes['api_error'], 'errors': 'Username and password required'})

    conn = db_connection()
    cur = conn.cursor()

    try:
        cur.execute('SELECT user_id, password FROM users WHERE username = %s', (username,))
        user = cur.fetchone()

        if user and user[1] == password:
            auth_token = create_token(user[0])
            response = {'status': StatusCodes['success'], 'results': auth_token}
        else:
            response = {'status': StatusCodes['api_error'], 'errors': 'Invalid credentials'}

    except (Exception, psycopg2.DatabaseError) as error:
        logger.error(f'PUT /dbproj/login - error: {error}')
        response = {'status': StatusCodes['internal_error'], 'errors': str(error)}
        conn.rollback()
    finally:
        cur.close()
        conn.close()

    return flask.jsonify(response)

# Schedule Appointment
@app.route('/dbproj/appointment', methods=['POST'])
@token_required
def schedule_appointment(current_user_id):
    logger.info('POST /dbproj/appointment')
    payload = flask.request.get_json()

    required_fields = ['doctor_id', 'date']
    for field in required_fields:
        if field not in payload:
            response = {'status': StatusCodes['api_error'], 'errors': f'{field} value not in payload'}
            return flask.jsonify(response)

    conn = db_connection()
    cur = conn.cursor()

    try:

        # Check if current user is a patient
        cur.execute('SELECT users_user_id FROM patient WHERE users_user_id = %s', (current_user_id,))
        patient = cur.fetchone()

        if not patient:
            response = {'status': StatusCodes['api_error'], 'errors': 'User is not a patient'}
            return flask.jsonify(response)
        


        # Insert appointment
        cur.execute('INSERT INTO event (doctor_employee_users_user_id, patient_users_user_id, event_date) VALUES (%s, %s, %s) RETURNING event_id',
                    (payload['doctor_id'], current_user_id, payload['date']))
        event_id = cur.fetchone()[0]


        cur.execute('INSERT INTO appointment (event_event_id) VALUES (%s)', (event_id,))

        conn.commit()
        response = {'status': StatusCodes['success'], 'results (event_id)': event_id}

    except (Exception, psycopg2.DatabaseError) as error:
        logger.error(f'POST /dbproj/appointment - error: {error}')
        response = {'status': StatusCodes['internal_error'], 'errors': str(error)}
        conn.rollback()
    finally:
        if conn is not None:
            conn.close()

    return flask.jsonify(response)

def is_assistant(user_id):
    conn = db_connection()
    cur = conn.cursor()

    try:
        cur.execute('SELECT * FROM assistant WHERE employee_users_user_id = %s', (user_id,))
        assistant = cur.fetchone()
        return assistant is not None
    except psycopg2.Error as e:
        logger.error(f'Error checking if user is assistant: {e}')
        return False
    finally:
        if conn is not None:
            conn.close()


# See Appointments
@app.route('/dbproj/appointments/<patient_user_id>', methods=['GET'])
@token_required
def see_appointments(current_user_id, patient_user_id):
    logger.info(f'GET /dbproj/appointments/{patient_user_id}')

    patient_user_id=int(patient_user_id)
    # Verificar se o usuário atual é um assistente ou o próprio paciente
    if current_user_id != patient_user_id:    
        if not is_assistant(current_user_id):
            return flask.jsonify({'status': StatusCodes['api_error'], 'errors': 'Unauthorized access'})

    conn = db_connection()
    cur = conn.cursor()

    try:
        # Consulta para recuperar os compromissos do paciente específico
        cur.execute('''
            SELECT appointment.event_event_id, event.doctor_employee_users_user_id, event.event_date
            FROM appointment
            JOIN event ON appointment.event_event_id = event.event_id
            WHERE event.patient_users_user_id = %s
        ''', (patient_user_id,))
        
        appointments = cur.fetchall()
        
        # Formatar os resultados
        results = []
        for appointment in appointments:
            appointment_id, doctor_id, date = appointment
            results.append({
                'id': appointment_id,
                'doctor_id': doctor_id,
                'date': date.strftime('%Y-%m-%d %H:%M:%S')  # Formatar a data para string
            })

        response = {'status': StatusCodes['success'], 'results': results}

    except (Exception, psycopg2.DatabaseError) as error:
        logger.error(f'GET /dbproj/appointments/{patient_user_id} - error: {error}')
        response = {'status': StatusCodes['internal_error'], 'errors': str(error)}
    finally:
        if conn is not None:
            conn.close()

    return flask.jsonify(response)

# Schedule Surgery
@app.route('/dbproj/surgery', defaults={'hospitalization_id': None}, methods=['POST'])
@app.route('/dbproj/surgery/<hospitalization_id>', methods=['POST'])
@token_required
def schedule_surgery(current_user_id, hospitalization_id):
    logger.info('POST /dbproj/surgery')
    payload = flask.request.get_json()

    required_fields = ['patient_id', 'doctor', 'nurses', 'date']
    for field in required_fields:
        if field not in payload:
            response = {'status': StatusCodes['api_error'], 'errors': f'{field} value not in payload'}
            return flask.jsonify(response)

    conn = db_connection()
    cur = conn.cursor()

    try:
        # Check if current user is an assistant
        if not is_assistant(current_user_id):
            response = {'status': StatusCodes['api_error'], 'errors': 'Only assistants can schedule surgeries'}
            return flask.jsonify(response)
        
        
        
        # Check if hospitalization_id is provided
        if hospitalization_id:
            # Verify if the provided hospitalization_id exists
            cur.execute('SELECT hospitalization_id FROM hospitalization WHERE hospitalization_id = %s', (hospitalization_id,))
            result = cur.fetchone()
            if not result:
                response = {'status': StatusCodes['api_error'], 'errors': 'Invalid hospitalization_id'}
                return flask.jsonify(response)
            
            # ver se o paciente é o mesmo

            # Extract bill_id associated with the hospitalization
            cur.execute('SELECT bill_bill_id FROM hospitalization WHERE hospitalization_id = %s', (hospitalization_id,))
            bill_id_from_hospitalization = cur.fetchone()
            if bill_id_from_hospitalization:
                bill_id = bill_id_from_hospitalization[0]
            else:
                response = {'status': StatusCodes['api_error'], 'errors': 'No bill associated with the hospitalization'}
                return flask.jsonify(response)
            # Update the bill's final price
            cur.execute('UPDATE bill SET final_price = final_price + 120 WHERE bill_id = %s', (bill_id,))
            conn.commit()
            
          
        else:
            # Create new hospitalization
            cur.execute('INSERT INTO hospitalization (assistant_employee_users_user_id) VALUES (%s) RETURNING hospitalization_id',
                        (current_user_id,))
            hospitalization_id = cur.fetchone()[0]
        # Insert surgery event
        cur.execute('INSERT INTO event (event_date, doctor_employee_users_user_id, patient_users_user_id) VALUES (%s, %s, %s) RETURNING event_id',
                    (payload['date'], payload['doctor'], payload['patient_id']))
        event_id = cur.fetchone()[0]

        # Associate event with hospitalization
        cur.execute('INSERT INTO surgery (hospitalization_hospitalization_id, event_event_id) VALUES (%s, %s)',
                    (hospitalization_id, event_id))

        # Insert nurses
        for nurse_id, role in payload['nurses']:
            cur.execute('INSERT INTO event_nurse (event_event_id, nurse_employee_users_user_id) VALUES (%s, %s)',
                        (event_id, nurse_id))

        conn.commit()
        response = {
            'status': StatusCodes['success'],
            'results': {
                'hospitalization_id': hospitalization_id,
                'surgery_id': event_id,
                'patient_id': payload['patient_id'],
                'doctor_id': payload['doctor'],
                'date': payload['date']
            }
        }

    except (Exception, psycopg2.DatabaseError) as error:
        logger.error(f'POST /dbproj/surgery - error: {error}')
        response = {'status': StatusCodes['internal_error'], 'errors': str(error)}
        conn.rollback()
    finally:
        if conn is not None:
            conn.close()

    return flask.jsonify(response)

# Add Prescriptions
@app.route('/dbproj/prescription', methods=['POST'])
@token_required
def add_prescription(current_user_id):
    logger.info('POST /dbproj/prescription')
    payload = flask.request.get_json()

    required_fields = ['type', 'event_id', 'validity', 'medicines']
    for field in required_fields:
        if field not in payload:
            response = {'status': StatusCodes['api_error'], 'errors': f'{field} value not in payload'}
            return flask.jsonify(response)

    conn = db_connection()
    cur = conn.cursor()

    try:
        # Verify if the current user is a doctor
        cur.execute('SELECT * FROM doctor WHERE employee_users_user_id = %s', (current_user_id,))
        doctor = cur.fetchone()

        if not doctor:
            response = {'status': StatusCodes['api_error'], 'errors': 'Only doctors can add prescriptions'}
            return flask.jsonify(response)

        cur.execute('INSERT INTO prescription (validity) VALUES (%s) RETURNING prescription_id', (payload['validity'],))
        prescription_id = cur.fetchone()[0]
        
        # Associate prescription with medicines
        for medicine in payload['medicines']:
            cur.execute('SELECT medicine_id FROM medicine WHERE name = %s', (medicine['medicine'],))
            medicine_row = cur.fetchone()
            if medicine_row:
                medicine_id = medicine_row[0]
                cur.execute('INSERT INTO prescription_medicine (prescription_prescription_id, medicine_medicine_id) VALUES (%s, %s)',
                            (prescription_id, medicine_id))
            else:
                response = {'status': StatusCodes['api_error'], 'errors': f'Medicine {medicine["medicine"]} does not exist'}
                return flask.jsonify(response)

        # Associate prescription with event (appointment/hospitalization)
        if payload['type'] == 'appointment':
            cur.execute('INSERT INTO appointment_prescription (appointment_event_event_id, prescription_prescription_id) VALUES (%s, %s)',
                        (payload['event_id'], prescription_id))
        elif payload['type'] == 'hospitalization':
            cur.execute('INSERT INTO hospitalization_prescription (hospitalization_hospitalization_id, prescription_prescription_id) VALUES (%s, %s)',
                        (payload['event_id'], prescription_id))
        else:
            response = {'status': StatusCodes['api_error'], 'errors': 'Invalid type value'}
            return flask.jsonify(response)

        conn.commit()
        response = {'status': StatusCodes['success'], 'results': {'prescription_id': prescription_id}}

    except (Exception, psycopg2.DatabaseError) as error:
        logger.error(f'POST /dbproj/prescription - error: {error}')
        response = {'status': StatusCodes['internal_error'], 'errors': str(error)}
        conn.rollback()
    finally:
        if conn is not None:
            conn.close()

    return flask.jsonify(response)


# Get Prescriptions
@app.route('/dbproj/prescriptions/<person_id>', methods=['GET'])
@token_required
def get_prescriptions(current_user_id, person_id):
    logger.info(f'GET /dbproj/prescriptions/{person_id}')

    person_id = int(person_id)
    
    # Verificar se o usuário atual é um paciente
    if current_user_id != person_id and is_patient(current_user_id):
        return flask.jsonify({'status': StatusCodes['api_error'], 'errors': 'Unauthorized access'})

    conn = db_connection()
    cur = conn.cursor()

    try:
        # Consulta para recuperar as prescrições do paciente específico
        # Consulta para recuperar as prescrições do paciente específico
        cur.execute('''
            SELECT prescription.prescription_id, prescription.validity, medicine.name AS medicine_name
            FROM prescription
            JOIN prescription_medicine ON prescription.prescription_id = prescription_medicine.prescription_prescription_id
            JOIN medicine ON prescription_medicine.medicine_medicine_id = medicine.medicine_id
            JOIN event ON event.event_id IN (
                SELECT appointment_prescription.appointment_event_event_id
                FROM appointment_prescription
                WHERE prescription.prescription_id = appointment_prescription.prescription_prescription_id
                UNION ALL
                SELECT hospitalization_prescription.hospitalization_hospitalization_id
                FROM hospitalization_prescription
                WHERE prescription.prescription_id = hospitalization_prescription.prescription_prescription_id
            )
            WHERE event.patient_users_user_id = %s
        ''', (person_id,))




        prescriptions = cur.fetchall()
        
        # Formatar os resultados
        results = []
        for prescription in prescriptions:
            prescription_id, validity, medicine_name = prescription
            results.append({
                'id': prescription_id,
                'validity': validity.strftime('%Y-%m-%d'),  # Formatar a data para string
                'posology': {
                    'medicine': medicine_name,
                    'dose': 'placeholder',
                    'frequency': 'placeholder'
                }
            })

        response = {'status': StatusCodes['success'], 'results': results}

    except (Exception, psycopg2.DatabaseError) as error:
        logger.error(f'GET /dbproj/prescriptions/{person_id} - error: {error}')
        response = {'status': StatusCodes['internal_error'], 'errors': str(error)}
    finally:
        if conn is not None:
            conn.close()

    return flask.jsonify(response)


def is_patient(user_id):
    conn = db_connection()
    cur = conn.cursor()

    try:
        cur.execute('SELECT * FROM patient WHERE users_user_id = %s', (user_id,))
        patient = cur.fetchone()
        return patient is not None
    except psycopg2.Error as e:
        logger.error(f'Error checking if user is patient: {e}')
        return False
    finally:
        if conn is not None:
            conn.close()


def is_user_associated_with_bill(current_user_id, bill_id):
    conn = db_connection()
    cur = conn.cursor()
    
    try:
        # Check if bill is associated with an appointment
        cur.execute("""
            SELECT e.patient_users_user_id
            FROM bill b
            JOIN appointment a ON b.bill_id = a.bill_bill_id
            JOIN event e ON a.event_event_id = e.event_id
            WHERE b.bill_id = %s
        """, (bill_id,))
        patient_id = cur.fetchone()
        
        if patient_id and patient_id[0] == current_user_id:
            return True

        # Check if bill is associated with a hospitalization
        cur.execute("""
            SELECT e.patient_users_user_id
            FROM bill b
            JOIN hospitalization h ON b.bill_id = h.bill_bill_id
            JOIN surgery s ON h.hospitalization_id = s.hospitalization_hospitalization_id
            JOIN event e ON s.event_event_id = e.event_id
            WHERE b.bill_id = %s
        """, (bill_id,))
        patient_id = cur.fetchone()
        
        if patient_id and patient_id[0] == current_user_id:
            return True

        return False

    except (Exception, psycopg2.DatabaseError) as error:
        logger.error(f'Error verifying bill association for user {current_user_id} and bill {bill_id}: {error}')
        return False

    finally:
        if conn is not None:
            conn.close()


# Execute Payment
@app.route('/dbproj/bills/<int:bill_id>', methods=['POST'])
@token_required
def execute_payment(current_user_id, bill_id):
    logger.info(f'POST /dbproj/bills/{bill_id}')
    payload = flask.request.get_json()

    required_fields = ['amount', 'payment_method']
    for field in required_fields:
        if field not in payload:
            response = {'status': StatusCodes['api_error'], 'errors': f'{field} value not in payload'}
            return flask.jsonify(response)

    conn = db_connection()
    cur = conn.cursor()

    try:
        # Verificar se o usuário atual é um paciente
        if not is_patient(current_user_id) or not is_user_associated_with_bill(current_user_id, bill_id):
            response = {'status': StatusCodes['api_error'], 'errors': 'Only patients can pay their own bills'}
            return flask.jsonify(response)

        # Verificar se a conta existe
        cur.execute('SELECT * FROM bill WHERE bill_id = %s', (bill_id,))
        bill = cur.fetchone()
        if not bill:
            response = {'status': StatusCodes['api_error'], 'errors': 'Invalid bill_id'}
            return flask.jsonify(response)

        # Verificar se a conta já foi paga
        if bill[1] == 0:
            response = {'status': StatusCodes['api_error'], 'errors': 'Bill is already paid'}
            return flask.jsonify(response)

        # Calcular o valor restante após o pagamento
        remaining_value = bill[1] - payload['amount']
        

        # Inserir o pagamento
        cur.execute('INSERT INTO payment (payment_value, payment_method, payment_date, bill_bill_id) VALUES (%s, %s, %s, %s) RETURNING payment_id',
                    (payload['amount'], payload['payment_method'], datetime.now(), bill_id))
        payment_id = cur.fetchone()[0]

        # Atualizar o status da conta para "pago" se o valor restante for 0
        if remaining_value <= 0:
            cur.execute('UPDATE payment SET payment_value = %s WHERE payment_id = %s', (bill[1], payment_id))
            remaining_value = 0
        
        cur.execute('UPDATE bill SET final_price = %s WHERE bill_id = %s', (remaining_value, bill_id,))

        conn.commit()
        response = {
            'status': StatusCodes['success'],
            'results': {'remaining_value': remaining_value}
        }

    except (Exception, psycopg2.DatabaseError) as error:
        logger.error(f'POST /dbproj/bills/{bill_id}/payments - error: {error}')
        response = {'status': StatusCodes['internal_error'], 'errors': str(error)}
        conn.rollback()
    finally:
        if conn is not None:
            conn.close()

    return flask.jsonify(response)



@app.route('/dbproj/top3', methods=['GET'])
@token_required
def list_top3_patients(current_user_id):
    logger.info('GET /dbproj/top3')

    # Verificar se o usuário atual é um assistente
    if not is_assistant(current_user_id):
        return flask.jsonify({'status': StatusCodes['api_error'], 'errors': 'Only assistants can access this information'})

    conn = db_connection()
    cur = conn.cursor()

    try:
        # Executar a consulta SQL
        cur.execute('''
            SELECT p.username AS patient_name, 
                   SUM(pm.payment_value) AS total_payments,
                   json_agg(json_build_object(
                       'payment_id', pm.payment_id,
                       'amount', pm.payment_value,
                       'method', pm.payment_method,
                       'date', pm.payment_date
                   )) AS payments
            FROM patient pt
            JOIN users p ON pt.users_user_id = p.user_id
            JOIN event e ON e.patient_users_user_id = pt.users_user_id
            JOIN (
                SELECT h.bill_bill_id, e.event_id
                FROM hospitalization h
                JOIN surgery s ON h.hospitalization_id = s.hospitalization_hospitalization_id
                JOIN event e ON e.event_id = s.event_event_id
                UNION ALL
                SELECT a.bill_bill_id, e.event_id
                FROM appointment a
                JOIN event e ON e.event_id = a.event_event_id
            ) AS he ON e.event_id = he.event_id
            JOIN bill b ON b.bill_id = he.bill_bill_id
            JOIN payment pm ON pm.bill_bill_id = b.bill_id
            WHERE EXTRACT(MONTH FROM pm.payment_date) = EXTRACT(MONTH FROM CURRENT_DATE)
              AND EXTRACT(YEAR FROM pm.payment_date) = EXTRACT(YEAR FROM CURRENT_DATE)
            GROUP BY pt.users_user_id, p.username
            ORDER BY total_payments DESC
            LIMIT 3;
        ''')

        top_patients = cur.fetchall()
        results = []
        for patient in top_patients:
            patient_name, total_payments, payments = patient
            results.append({
                'patient_name': patient_name,
                'total_payments': total_payments,
                'payments': payments
            })

        response = {'status': StatusCodes['success'], 'results': results}

    except (Exception, psycopg2.DatabaseError) as error:
        logger.error(f'GET /dbproj/top3 - error: {error}')
        response = {'status': StatusCodes['internal_error'], 'errors': str(error)}
    finally:
        if conn is not None:
            conn.close()

    return flask.jsonify(response)




from datetime import datetime
@app.route('/dbproj/daily/<date>', methods=['GET'])
@token_required
def daily_summary(current_user_id, date):
    logger.info(f'GET /dbproj/daily/{date}')

    # Verificar se o usuário atual é um assistente
    if not is_assistant(current_user_id):
        return flask.jsonify({'status': StatusCodes['api_error'], 'errors': 'Only assistants can access this information'})

    conn = db_connection()
    cur = conn.cursor()

    try:
        # Consulta SQL para obter o resumo diário
        cur.execute('''
            SELECT 
                COALESCE(SUM(pm.payment_value), 0) AS amount_spent,
                COUNT(DISTINCT s.event_event_id) AS surgeries,
                COUNT(DISTINCT hp.prescription_prescription_id) AS prescriptions
            FROM hospitalization h
            LEFT JOIN surgery s ON h.hospitalization_id = s.hospitalization_hospitalization_id
            LEFT JOIN hospitalization_prescription hp ON h.hospitalization_id = hp.hospitalization_hospitalization_id
            LEFT JOIN event e_s ON s.event_event_id = e_s.event_id
            LEFT JOIN event e_hp ON hp.hospitalization_hospitalization_id = h.hospitalization_id
            LEFT JOIN payment pm ON h.bill_bill_id = pm.bill_bill_id
            WHERE 
                DATE(pm.payment_date) = %s 
                OR DATE(e_s.event_date) = %s 
                OR DATE(e_hp.event_date) = %s
        ''', (date, date, date))

        result = cur.fetchone()
        response = {
            'status': StatusCodes['success'],
            'results': {
                'amount_spent': result[0] if result[0] is not None else 0,
                'surgeries': result[1],
                'prescriptions': result[2]
            }
        }

    except (Exception, psycopg2.DatabaseError) as error:
        logger.error(f'GET /dbproj/daily/{date} - error: {error}')
        response = {'status': StatusCodes['internal_error'], 'errors': str(error)}
    finally:
        if conn is not None:
            conn.close()

    return flask.jsonify(response)


from datetime import datetime, timedelta

# Generate Monthly Report
@app.route('/dbproj/report', methods=['GET'])
@token_required
def generate_report(current_user_id):
    logger.info('GET /dbproj/report')

    # Check if current user is an assistant
    if not is_assistant(current_user_id):
        return flask.jsonify({'status': StatusCodes['api_error'], 'errors': 'Only assistants can generate reports'})

    conn = db_connection()
    cur = conn.cursor()

    try:
        # SQL query to get the doctor with the most surgeries each month in the last 12 months
        query = '''
        SELECT 
            TO_CHAR(event.event_date, 'YYYY-MM') AS month,
            doctor.employee_users_user_id AS doctor_id,
            COUNT(surgery.event_event_id) AS total_surgeries
        FROM 
            surgery
        JOIN 
            event ON surgery.event_event_id = event.event_id
        JOIN 
            doctor ON event.doctor_employee_users_user_id = doctor.employee_users_user_id
        WHERE 
            event.event_date >= NOW() - INTERVAL '1 year'
        GROUP BY 
            month, doctor.employee_users_user_id
        ORDER BY 
            month DESC, total_surgeries DESC
        '''
        cur.execute(query)
        results = cur.fetchall()

        # Process the results to keep only the top doctor for each month
        report = []
        month_tracker = None

        for row in results:
            month, doctor_id, total_surgeries = row
            if month != month_tracker:
                month_tracker = month
                report.append({
                    'month': month,
                    'doctor': doctor_id,
                    'surgeries': total_surgeries
                })

        response = {'status': StatusCodes['success'], 'results': report}

    except (Exception, psycopg2.DatabaseError) as error:
        logger.error(f'GET /dbproj/report - error: {error}')
        response = {'status': StatusCodes['internal_error'], 'errors': str(error)}
    finally:
        if conn is not None:
            conn.close()

    return flask.jsonify(response)





if __name__ == '__main__':
    # set up logging
    logging.basicConfig(filename='log_file.log')
    logger = logging.getLogger('logger')
    logger.setLevel(logging.DEBUG)
    ch = logging.StreamHandler()
    ch.setLevel(logging.DEBUG)

    # create formatter
    formatter = logging.Formatter('%(asctime)s [%(levelname)s]:  %(message)s', '%H:%M:%S')
    ch.setFormatter(formatter)
    logger.addHandler(ch)

    host = '127.0.0.1'
    port = 8080
    app.run(host=host, debug=True, threaded=True, port=port)
    logger.info(f'API v1.0 online: http://{host}:{port}')