\echo '============Creating SYSTEM user===========';
CREATE ROLE SYSTEM WITH LOGIN PASSWORD 'SYSTEM';

ALTER ROLE SYSTEM CREATEDB;

\echo '============Creating database TSS_DEV===========';
CREATE DATABASE TSS_DEV;

\echo '============Granting ALL privileges of TSS_DEV to SYSTEM===========';
GRANT ALL PRIVILEGES ON DATABASE TSS_DEV TO system;
