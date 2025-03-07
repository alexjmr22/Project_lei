CREATE TABLE patient (
	users_user_id INTEGER,
	PRIMARY KEY(users_user_id)
);

CREATE TABLE employee (
	users_user_id INTEGER,
	PRIMARY KEY(users_user_id)
);

CREATE TABLE doctor (
	medical_license	 INTEGER NOT NULL,
	employee_users_user_id INTEGER,
	PRIMARY KEY(employee_users_user_id)
);

CREATE TABLE nurse (
	category		 VARCHAR(512) NOT NULL,
	role			 VARCHAR(512),
	employee_users_user_id INTEGER,
	PRIMARY KEY(employee_users_user_id)
);

CREATE TABLE assistant (
	employee_users_user_id INTEGER,
	PRIMARY KEY(employee_users_user_id)
);

CREATE TABLE appointment (
	bill_bill_id	 INTEGER NOT NULL,
	event_event_id INTEGER,
	PRIMARY KEY(event_event_id)
);

CREATE TABLE surgery (
	hospitalization_hospitalization_id INTEGER NOT NULL,
	event_event_id			 INTEGER,
	PRIMARY KEY(event_event_id)
);

CREATE TABLE hospitalization (
	hospitalization_id		 INTEGER,
	bill_bill_id			 INTEGER NOT NULL,
	assistant_employee_users_user_id INTEGER NOT NULL,
	PRIMARY KEY(hospitalization_id)
);

CREATE TABLE prescription (
	prescription_id INTEGER,
	validity	 DATE,
	PRIMARY KEY(prescription_id)
);

CREATE TABLE medicine (
	medicine_id INTEGER,
	name	 VARCHAR(512) NOT NULL,
	PRIMARY KEY(medicine_id)
);

CREATE TABLE side_effect (
	side_effect_id	 INTEGER,
	name		 VARCHAR(512) NOT NULL,
	medicine_medicine_id INTEGER,
	PRIMARY KEY(side_effect_id,medicine_medicine_id)
);

CREATE TABLE bill (
	bill_id	 INTEGER,
	final_price INTEGER NOT NULL,
	PRIMARY KEY(bill_id)
);

CREATE TABLE payment (
	payment_id	 INTEGER,
	payment_value	 INTEGER NOT NULL,
	payment_method VARCHAR(512) NOT NULL,
	payment_date	 DATE NOT NULL,
	bill_bill_id	 INTEGER NOT NULL,
	PRIMARY KEY(payment_id)
);

CREATE TABLE specialization (
	specialization_id INTEGER,
	name		 VARCHAR(512) NOT NULL,
	PRIMARY KEY(specialization_id)
);

CREATE TABLE event (
	event_id			 INTEGER,
	event_date			 DATE NOT NULL,
	doctor_employee_users_user_id INTEGER NOT NULL,
	patient_users_user_id	 INTEGER NOT NULL,
	PRIMARY KEY(event_id)
);

CREATE TABLE users (
	user_id	 INTEGER,
	username VARCHAR(512) NOT NULL,
	password VARCHAR(512) NOT NULL,
	contact	 VARCHAR(512) NOT NULL,
	PRIMARY KEY(user_id)
);

CREATE TABLE dosage (
	quantity		 FLOAT(8) NOT NULL,
	medicine_medicine_id INTEGER,
	PRIMARY KEY(medicine_medicine_id)
);

CREATE TABLE severity (
	severity			 VARCHAR(512) NOT NULL,
	side_effect_side_effect_id	 INTEGER,
	side_effect_medicine_medicine_id INTEGER,
	PRIMARY KEY(side_effect_side_effect_id,side_effect_medicine_medicine_id)
);

CREATE TABLE probability (
	probability			 FLOAT(8) NOT NULL,
	side_effect_side_effect_id	 INTEGER,
	side_effect_medicine_medicine_id INTEGER,
	PRIMARY KEY(side_effect_side_effect_id,side_effect_medicine_medicine_id)
);

CREATE TABLE frequency (
	frequency		 INTEGER,
	medicine_medicine_id INTEGER,
	PRIMARY KEY(frequency,medicine_medicine_id)
);

CREATE TABLE hospitalization_patient (
	hospitalization_hospitalization_id INTEGER,
	patient_users_user_id		 INTEGER NOT NULL,
	PRIMARY KEY(hospitalization_hospitalization_id)
);

CREATE TABLE appointment_prescription (
	appointment_event_event_id	 INTEGER,
	prescription_prescription_id INTEGER NOT NULL,
	PRIMARY KEY(appointment_event_event_id)
);

CREATE TABLE prescription_medicine (
	prescription_prescription_id INTEGER,
	medicine_medicine_id	 INTEGER,
	PRIMARY KEY(prescription_prescription_id,medicine_medicine_id)
);

CREATE TABLE hospitalization_prescription (
	hospitalization_hospitalization_id INTEGER,
	prescription_prescription_id	 INTEGER NOT NULL,
	PRIMARY KEY(hospitalization_hospitalization_id)
);

CREATE TABLE specialization_specialization (
	specialization_specialization_id	 INTEGER,
	specialization_specialization_id1 INTEGER NOT NULL,
	PRIMARY KEY(specialization_specialization_id)
);

CREATE TABLE specialization_doctor (
	specialization_specialization_id INTEGER,
	doctor_employee_users_user_id	 INTEGER,
	PRIMARY KEY(specialization_specialization_id,doctor_employee_users_user_id)
);

CREATE TABLE event_nurse (
	event_event_id		 INTEGER NOT NULL,
	nurse_employee_users_user_id INTEGER,
	PRIMARY KEY(nurse_employee_users_user_id)
);

ALTER TABLE patient ADD CONSTRAINT patient_fk1 FOREIGN KEY (users_user_id) REFERENCES users(user_id);
ALTER TABLE employee ADD CONSTRAINT employee_fk1 FOREIGN KEY (users_user_id) REFERENCES users(user_id);
ALTER TABLE doctor ADD CONSTRAINT doctor_fk1 FOREIGN KEY (employee_users_user_id) REFERENCES employee(users_user_id);
ALTER TABLE nurse ADD CONSTRAINT nurse_fk1 FOREIGN KEY (employee_users_user_id) REFERENCES employee(users_user_id);
ALTER TABLE assistant ADD CONSTRAINT assistant_fk1 FOREIGN KEY (employee_users_user_id) REFERENCES employee(users_user_id);
ALTER TABLE appointment ADD UNIQUE (bill_bill_id);
ALTER TABLE appointment ADD CONSTRAINT appointment_fk1 FOREIGN KEY (bill_bill_id) REFERENCES bill(bill_id);
ALTER TABLE appointment ADD CONSTRAINT appointment_fk2 FOREIGN KEY (event_event_id) REFERENCES event(event_id);
ALTER TABLE surgery ADD CONSTRAINT surgery_fk1 FOREIGN KEY (hospitalization_hospitalization_id) REFERENCES hospitalization(hospitalization_id);
ALTER TABLE surgery ADD CONSTRAINT surgery_fk2 FOREIGN KEY (event_event_id) REFERENCES event(event_id);
ALTER TABLE hospitalization ADD UNIQUE (bill_bill_id);
ALTER TABLE hospitalization ADD CONSTRAINT hospitalization_fk1 FOREIGN KEY (bill_bill_id) REFERENCES bill(bill_id);
ALTER TABLE hospitalization ADD CONSTRAINT hospitalization_fk2 FOREIGN KEY (assistant_employee_users_user_id) REFERENCES assistant(employee_users_user_id);
ALTER TABLE side_effect ADD CONSTRAINT side_effect_fk1 FOREIGN KEY (medicine_medicine_id) REFERENCES medicine(medicine_id);
ALTER TABLE payment ADD CONSTRAINT payment_fk1 FOREIGN KEY (bill_bill_id) REFERENCES bill(bill_id);
ALTER TABLE event ADD UNIQUE (doctor_employee_users_user_id, patient_users_user_id);
ALTER TABLE event ADD CONSTRAINT event_fk1 FOREIGN KEY (doctor_employee_users_user_id) REFERENCES doctor(employee_users_user_id);
ALTER TABLE event ADD CONSTRAINT event_fk2 FOREIGN KEY (patient_users_user_id) REFERENCES patient(users_user_id);
ALTER TABLE users ADD UNIQUE (username, contact);
ALTER TABLE dosage ADD CONSTRAINT dosage_fk1 FOREIGN KEY (medicine_medicine_id) REFERENCES medicine(medicine_id);
ALTER TABLE severity ADD CONSTRAINT severity_fk1 FOREIGN KEY (side_effect_side_effect_id, side_effect_medicine_medicine_id) REFERENCES side_effect(side_effect_id, medicine_medicine_id);
ALTER TABLE probability ADD CONSTRAINT probability_fk1 FOREIGN KEY (side_effect_side_effect_id, side_effect_medicine_medicine_id) REFERENCES side_effect(side_effect_id, medicine_medicine_id);
ALTER TABLE frequency ADD CONSTRAINT frequency_fk1 FOREIGN KEY (medicine_medicine_id) REFERENCES medicine(medicine_id);
ALTER TABLE hospitalization_patient ADD UNIQUE (patient_users_user_id);
ALTER TABLE hospitalization_patient ADD CONSTRAINT hospitalization_patient_fk1 FOREIGN KEY (hospitalization_hospitalization_id) REFERENCES hospitalization(hospitalization_id);
ALTER TABLE hospitalization_patient ADD CONSTRAINT hospitalization_patient_fk2 FOREIGN KEY (patient_users_user_id) REFERENCES patient(users_user_id);
ALTER TABLE appointment_prescription ADD UNIQUE (prescription_prescription_id);
ALTER TABLE appointment_prescription ADD CONSTRAINT appointment_prescription_fk1 FOREIGN KEY (appointment_event_event_id) REFERENCES appointment(event_event_id);
ALTER TABLE appointment_prescription ADD CONSTRAINT appointment_prescription_fk2 FOREIGN KEY (prescription_prescription_id) REFERENCES prescription(prescription_id);
ALTER TABLE prescription_medicine ADD CONSTRAINT prescription_medicine_fk1 FOREIGN KEY (prescription_prescription_id) REFERENCES prescription(prescription_id);
ALTER TABLE prescription_medicine ADD CONSTRAINT prescription_medicine_fk2 FOREIGN KEY (medicine_medicine_id) REFERENCES medicine(medicine_id);
ALTER TABLE hospitalization_prescription ADD UNIQUE (prescription_prescription_id);
ALTER TABLE hospitalization_prescription ADD CONSTRAINT hospitalization_prescription_fk1 FOREIGN KEY (hospitalization_hospitalization_id) REFERENCES hospitalization(hospitalization_id);
ALTER TABLE hospitalization_prescription ADD CONSTRAINT hospitalization_prescription_fk2 FOREIGN KEY (prescription_prescription_id) REFERENCES prescription(prescription_id);
ALTER TABLE specialization_specialization ADD CONSTRAINT specialization_specialization_fk1 FOREIGN KEY (specialization_specialization_id) REFERENCES specialization(specialization_id);
ALTER TABLE specialization_specialization ADD CONSTRAINT specialization_specialization_fk2 FOREIGN KEY (specialization_specialization_id1) REFERENCES specialization(specialization_id);
ALTER TABLE specialization_doctor ADD CONSTRAINT specialization_doctor_fk1 FOREIGN KEY (specialization_specialization_id) REFERENCES specialization(specialization_id);
ALTER TABLE specialization_doctor ADD CONSTRAINT specialization_doctor_fk2 FOREIGN KEY (doctor_employee_users_user_id) REFERENCES doctor(employee_users_user_id);
ALTER TABLE event_nurse ADD CONSTRAINT event_nurse_fk1 FOREIGN KEY (event_event_id) REFERENCES event(event_id);
ALTER TABLE event_nurse ADD CONSTRAINT event_nurse_fk2 FOREIGN KEY (nurse_employee_users_user_id) REFERENCES nurse(employee_users_user_id);

