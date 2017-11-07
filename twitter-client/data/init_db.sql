\echo '============Creating SYSTEM user===========';
CREATE ROLE SYSTEM WITH LOGIN PASSWORD 'SYSTEM';

ALTER ROLE SYSTEM CREATEDB;

\echo '============Creating database CS_776===========';
CREATE DATABASE CS_776;

\echo '============Granting ALL privileges of CS_776 to SYSTEM===========';
GRANT ALL PRIVILEGES ON DATABASE CS_776 TO system;
