-- Create a new database
CREATE DATABASE IF NOT EXISTS iot;

-- Switch to the newly created database
USE iot;

-- Create the 'company' table
CREATE TABLE company (
    id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(255) NOT NULL
);

-- Create the 'user' table
CREATE TABLE user (
    id INT AUTO_INCREMENT PRIMARY KEY,
    username VARCHAR(255) NOT NULL,
    password VARCHAR(255) NOT NULL,
    database_token VARCHAR(255) NOT NULL,
    first_name VARCHAR(255),
    last_name VARCHAR(255),
    privileges VARCHAR(255)
);

-- Create the 'gateway' table
CREATE TABLE gateway (
    id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(255) NOT NULL,
    placement VARCHAR(255),
    company_id INT,
    FOREIGN KEY (company_id) REFERENCES company(id)
);

-- Create the 'device' table
CREATE TABLE device (
    id INT AUTO_INCREMENT PRIMARY KEY,
    device_type INT,
    device_number INT,
    device_name VARCHAR(255) NOT NULL,
    placement VARCHAR(255),
    gateway_id INT,
    FOREIGN KEY (gateway_id) REFERENCES gateway(id)
);

CREATE TABLE employed (
    user_id INT,
    company_id INT,
    PRIMARY KEY (user_id, company_id),
    FOREIGN KEY (user_id) REFERENCES user(id),
    FOREIGN KEY (company_id) REFERENCES company(id)
);

INSERT INTO company (id, name) VALUES 
(1, 'company1'),
(2, 'company2');

INSERT INTO user (id, username, password, database_token, first_name, last_name, privileges) VALUES
(1, 'john_doe', 'pass', 'abc123', 'John', 'Doe', 'admin'),
(2, 'steve_elan', 'pass', 'abc123', 'John', 'Doe', 'admin');

INSERT INTO employed (user_id, company_id) VALUES 
(1, 1),
(1, 2),
(2, 2);

INSERT INTO gateway (id, name, placement, company_id) VALUES
(1, 'gateway1', 'Office Building A', 1),
(2, 'gateway2', 'Office Building ', 2);

INSERT INTO device (id, device_type, device_number, device_name, placement, gateway_id) VALUES 
(1, 1, 1, 'sklep1', 'Floor 1, Room 102', 1),
(2, 1, 2, 'sklep2', 'Floor 1, Room 103', 1),
(3, 1, 3, 'sklep3', 'Floor 1, Room 104', 1),
(4, 2, 4, 'ul1', 'Floor 2, Room 201', 2);
