# Twitter Client wrapper

* install https://github.com/bear/python-twitter
* add **twitter-client.properties** to 'ROOT_DIR'. Required properties:
  * api-key=?????
  * api-secret=?????
  * access-token-key=????
  * access-token-secret=????
* setting up db
  * install postgresql https://www.codementor.io/devops/tutorial/getting-started-postgresql-server-mac-osx
  * cd into twitter-client/data
  * login as postgres: "psql postgres"
  * run init_db.sql ("\i init_db.sql")
    * verify system role is created ("\du") and tss_dev db is created ("\list")
  * logout and login as system: "psql postgres -U system"
  * run create_db_schema.sql ("\i create_db_schema.sql")
    * verify schema and table created:
      * switch to tss_dev schema ("set search_path to tss_dev;") and verify ("show search_path;")
      * verify table is created ("\dt")
  * quit ("\q")
* DataGrip connection info
  * host: localhost, database: tss_dev, user: system, password: system, port: 5432
  * url: jdbc:postgresql://localhost:5432/tss_dev
  * schema: cs_776

