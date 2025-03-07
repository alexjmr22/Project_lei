-- Create a sequence for events_id
CREATE SEQUENCE events_id_seq;

-- Create a trigger function to set the events_id
CREATE OR REPLACE FUNCTION set_events_id()
RETURNS TRIGGER AS $$
BEGIN
    IF NEW.event_id IS NULL THEN
        NEW.event_id := nextval('events_id_seq');
    END IF;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

-- Attach the trigger function to the events table
CREATE TRIGGER set_events_id_trigger
BEFORE INSERT ON event
FOR EACH ROW
EXECUTE FUNCTION set_events_id();

----------------------------------------------------------------

-- Create a sequence for hospitalization_id
CREATE SEQUENCE hospitalization_id_seq;

-- Create a trigger function to set the hospitalization_id
CREATE OR REPLACE FUNCTION set_hospitalization_id()
RETURNS TRIGGER AS $$
BEGIN
    IF NEW.hospitalization_id IS NULL THEN
        NEW.hospitalization_id := nextval('hospitalization_id_seq');
    END IF;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

-- Attach the trigger function to the hospitalization table
CREATE TRIGGER set_hospitalization_id_trigger
BEFORE INSERT ON hospitalization
FOR EACH ROW
EXECUTE FUNCTION set_hospitalization_id();

----------------------------------------------------------------

-- Create a sequence for user_id
CREATE SEQUENCE users_id_seq;

-- Create a trigger function to set the user_id
CREATE OR REPLACE FUNCTION set_user_id()
RETURNS TRIGGER AS $$
BEGIN
    IF NEW.user_id IS NULL THEN
        NEW.user_id := nextval('users_id_seq');
    END IF;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

-- Attach the trigger function to the users table
CREATE TRIGGER set_user_id_trigger
BEFORE INSERT ON users
FOR EACH ROW
EXECUTE FUNCTION set_user_id();

----------------------------------------------------------------


-- Criação de sequence para bill_id
CREATE SEQUENCE bill_id_sequence;

-- Trigger function to create a bill for each appointment
CREATE OR REPLACE FUNCTION create_bill_for_appointment()
RETURNS TRIGGER AS $$
BEGIN
    -- Insert a new bill with a default final price of 50 and get its id
    INSERT INTO bill (bill_id, final_price) VALUES (NEXTVAL('bill_id_sequence'), 50) RETURNING bill_id INTO NEW.bill_bill_id;

    -- Return the new row to be inserted into the appointment table
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

-- Trigger to execute the function before inserting into the appointment table
CREATE TRIGGER before_appointment_insert
BEFORE INSERT ON appointment
FOR EACH ROW
EXECUTE FUNCTION create_bill_for_appointment();



-- Trigger function to create a bill for each surgery
CREATE OR REPLACE FUNCTION create_bill_for_surgery()
RETURNS TRIGGER AS $$
BEGIN
    -- Insert a new bill with a default final price of 120 and get its id
    INSERT INTO bill (bill_id, final_price) VALUES (NEXTVAL('bill_id_sequence'), 120) RETURNING bill_id INTO NEW.bill_bill_id;

    -- Return the new row to be inserted into the hospitalization table
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;


-- Trigger to execute the function before inserting into the appointment table
CREATE TRIGGER before_surgery_insert
BEFORE INSERT ON hospitalization
FOR EACH ROW
EXECUTE FUNCTION create_bill_for_surgery();



-- Adiciona medicamentos
INSERT INTO medicine (medicine_id, name) VALUES
    (1, 'Paracetamol'),
    (2, 'Ibuprofen');

-- Adiciona efeitos colaterais
INSERT INTO side_effect (side_effect_id, name, medicine_medicine_id) VALUES
    (1, 'Nausea', 1),
    (2, 'Headache', 2);

-- Adiciona severidade dos efeitos colaterais
INSERT INTO severity (severity, side_effect_side_effect_id, side_effect_medicine_medicine_id) VALUES
    ('Mild', 1, 1),
    ('Mild', 2, 2);

-- Adiciona probabilidade dos efeitos colaterais
INSERT INTO probability (probability, side_effect_side_effect_id, side_effect_medicine_medicine_id) VALUES
    (0.2, 1, 1),
    (0.3, 2, 2);

CREATE SEQUENCE prescription_sequence START 1;
ALTER TABLE prescription
    ALTER COLUMN prescription_id SET DEFAULT nextval('prescription_sequence');


----------------------------------------------------------------
CREATE SEQUENCE payment_id_sequence START 1;
ALTER TABLE payment
    ALTER COLUMN payment_id SET DEFAULT nextval('payment_id_sequence');