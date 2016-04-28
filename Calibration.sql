CREATE OR REPLACE FUNCTION getphotomultipliersdata(IN p_run_id integer)
  RETURNS TABLE(hvpmconnection_id integer, hvpmconnection_isrightside boolean, photomultiplier_id integer, photomultiplier_isactive boolean, photomultiplier_status character varying, photomultiplier_name character varying, photomultiplier_maxhv double precision, photomultiplier_description character varying, photomultiplier_producer character varying, photomultiplier_boughtdate timestamp without time zone, photomultiplier_serialnumber character varying, photomultiplier_takespositivevoltage boolean, pmmodel_id integer, pmmodel_name character varying, pmcalibration_id integer, pmcalibration_name character varying, pmcalibration_opthv real, pmcalibration_c2e_1 real, pmcalibration_c2e_2 real, pmcalibration_gainalpha real, pmcalibration_gainbeta real, user_id integer, user_name character varying, user_lastname character varying, user_login character varying, user_password character varying, user_isroot boolean, user_creationdate timestamp without time zone, user_lastlogindate timestamp without time zone, setup_id integer, run_id integer) AS
$BODY$
BEGIN
  FOR 
    hvpmconnection_id,
    hvpmconnection_isrightside,

    photomultiplier_id,
    photomultiplier_isactive,
    photomultiplier_status,
    photomultiplier_name,
    photomultiplier_maxhv,
    photomultiplier_description,
    photomultiplier_producer,
    photomultiplier_boughtdate,
    photomultiplier_serialnumber,
    photomultiplier_takespositivevoltage,

    pmmodel_id,
    pmmodel_name,

    user_id,
    user_name,
    user_lastname,
    user_login,
    user_password,
    user_isroot,
    user_creationdate,
    user_lastlogindate,

    setup_id,
    run_id

  IN

      SELECT
	"HVPMConnection".id AS hvpmconnection_id,
	"HVPMConnection".isrightside AS hvpmconnection_isrightside,

	"PhotoMultiplier".id AS photomultiplier_id,
	"PhotoMultiplier".isactive AS photomultiplier_isactive,
	"PhotoMultiplier".status AS photomultiplier_status,
	"PhotoMultiplier".name AS photomultiplier_name,
	"PhotoMultiplier".maxhv AS photomultiplier_maxhv,
	"PhotoMultiplier".description AS photomultiplier_description,
	"PhotoMultiplier".producer AS photomultiplier_producer,
	"PhotoMultiplier".boughtdate AS photomultiplier_boughtdate,
	"PhotoMultiplier".serialnumber AS photomultiplier_serialnumber,
	"PhotoMultiplier".takespositivevoltage AS photomultiplier_takespositivevoltage,

	"PhotoMultiplier".pmmodel_id AS pmmodel_id,
	"PMModel".name AS pmmodel_name,

	"PETUser".id AS user_id,
	"PETUser".name AS user_name,
	"PETUser".lastname AS user_lastname,
	"PETUser".login AS user_login,
	"PETUser".password AS user_password,
	"PETUser".isroot AS user_isroot,
	"PETUser".creationdate AS user_creationdate,
	"PETUser".lastlogindate AS user_lastlogindate,
	
	"Setup".id AS setup_id,
	"Run".id AS run_id

      FROM "HVPMConnection", "PhotoMultiplier", "PMModel", "PETUser", "Run", "Setup"
	WHERE
	  "HVPMConnection".photomultiplier_id = "PhotoMultiplier".id
	  AND
	  "HVPMConnection".setup_id = "Setup".id
	  AND
	  "Run".setup_id = "Setup".id
	  AND
	  "PhotoMultiplier".creator_id = "PETUser".id
	  AND
	  "Run".id = p_run_id
  LOOP
    RETURN NEXT;
  END LOOP;
END
$BODY$
  LANGUAGE plpgsql STABLE
  COST 100
  ROWS 1000;
ALTER FUNCTION getphotomultipliersdata(integer)
  OWNER TO postgres;


CREATE TABLE "CalibrationType" (
  id serial NOT NULL,
  name character varying(50),
  param_count integer NOT NULL,
  formula character varying(255) NOT NULL,
  CONSTRAINT "CalibrationType_id_key" PRIMARY KEY (id)
);
ALTER TABLE "CalibrationType"
  OWNER TO postgres;

CREATE TABLE "CalibratePhmAmplification_Values" (
  id serial NOT NULL,
  id_type integer NOT NULL,
  id_phm integer NOT NULL,
  param_values character varying(255),
  CONSTRAINT "CalibrationPhmAmplification_Values_id_key" PRIMARY KEY (id),
  CONSTRAINT "CalibrationPhmAmplification_Values_id_phm" FOREIGN KEY (id_phm)
      REFERENCES "PhotoMultiplier" (id) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION,
  CONSTRAINT "CalibrationPhmAmplification_Values_id_type" FOREIGN KEY (id_type)
      REFERENCES "CalibrationType" (id) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION
);
ALTER TABLE "CalibratePhmAmplification_Values"
  OWNER TO postgres;


CREATE TABLE "CalibratePhmAmplification_Connect" (
  id serial NOT NULL,
  id_cal integer NOT NULL,
  id_run integer NOT NULL,
  CONSTRAINT "CalibratePhmAmplification_Connect_id" PRIMARY KEY (id),
  CONSTRAINT "CalibratePhmAmplification_Connect_Run" FOREIGN KEY (id_run)
      REFERENCES "Run" (id) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION,
  CONSTRAINT "CalibratePhmAmplification_Connect_cal" FOREIGN KEY (id_cal)
      REFERENCES "CalibratePhmAmplification_Values" (id) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION
);
ALTER TABLE "CalibratePhmAmplification_Connect"
  OWNER TO postgres;



CREATE OR REPLACE FUNCTION getcalibrationtypes()
  RETURNS TABLE(type_id integer, name character varying, param_count integer, formula character varying) AS
$BODY$
begin
for type_id,name,param_count,formula in
select distinct
"CalibrationType".id as type_id,"CalibrationType".name,
"CalibrationType".param_count,
"CalibrationType".formula
from
"CalibrationType"
loop
return next;
end loop;
end
$BODY$
  LANGUAGE plpgsql STABLE
  COST 100
  ROWS 1000;
ALTER FUNCTION getcalibrationtypes()
  OWNER TO postgres;



CREATE OR REPLACE FUNCTION getcalibrations_phmampl_allphm(IN phm_id integer)
  RETURNS TABLE(cal_id integer, name character varying, param_count integer, formula character varying, param_values character varying) AS
$BODY$
begin
for cal_id, name,param_count,formula,param_values in
select distinct
"CalibratePhmAmplification_Values".id as cal_id,
"CalibratePhmAmplification_Values".id_type,
"CalibrationType".name,
"CalibrationType".param_count,
"CalibrationType".formula,
"CalibratePhmAmplification_Values".param_values
from
"CalibrationType","CalibratePhmAmplification_Values"
where
"CalibrationType".id = "CalibratePhmAmplification_Values".id_type and
"CalibratePhmAmplification_Values".id_phm=phm_id 
loop
return next;
end loop;
end
$BODY$
  LANGUAGE plpgsql STABLE
  COST 100
  ROWS 1000;
ALTER FUNCTION getcalibrations_phmampl_allphm(integer)
  OWNER TO postgres;


CREATE OR REPLACE FUNCTION getcalibrations_phmampl_run(IN run_id integer)
  RETURNS TABLE(id_phm integer, name character varying, param_count integer, formula character varying, param_values character varying) AS
$BODY$
begin
for id_phm, name,param_count,formula,param_values in
select distinct
"CalibratePhmAmplification_Connect".id,
"CalibratePhmAmplification_Values".id as cal_id,
"CalibratePhmAmplification_Connect".id_run,
"CalibratePhmAmplification_Values".id_phm,
"CalibrationType".name,
"CalibrationType".param_count,
"CalibrationType".formula,
"CalibratePhmAmplification_Values".param_values
from
"CalibrationType","CalibratePhmAmplification_Values","CalibratePhmAmplification_Connect"
where
"CalibratePhmAmplification_Connect".id_run= run_id and
"CalibrationType".id = "CalibratePhmAmplification_Values".id_type and
"CalibratePhmAmplification_Values".id = "CalibratePhmAmplification_Connect".id_cal 
loop
return next;
end loop;
end
$BODY$
  LANGUAGE plpgsql STABLE
  COST 100
  ROWS 1000;
ALTER FUNCTION getcalibrations_phmampl_run(integer)
  OWNER TO postgres;


CREATE OR REPLACE FUNCTION insert_calibrationtype(
    new_name character varying,
    new_paramcount integer,
    new_formula character varying)
  RETURNS void AS
$BODY$
begin
insert into "CalibrationType" (name,param_count,formula) values (new_name,new_paramcount,new_formula);
end
$BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100;
ALTER FUNCTION insert_calibrationtype(character varying, integer, character varying)
  OWNER TO postgres;



CREATE OR REPLACE FUNCTION insert_calibration_phmampl(
    phm_id integer,
    type_id integer,
    new_values character varying)
  RETURNS void AS
$BODY$
begin
insert into "CalibratePhmAmplification_Values" (id_type,id_phm,param_values) values (type_id,phm_id,new_values);
end
$BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100;
ALTER FUNCTION insert_calibration_phmampl(integer, integer, character varying)
  OWNER TO postgres;



CREATE OR REPLACE FUNCTION connect_calibration_phmampl(
    cal_id integer,
    run_id integer)
  RETURNS void AS
$BODY$
begin
insert into "CalibratePhmAmplification_Connect" (id_cal,id_run) values (cal_id, run_id);
end
$BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100;
ALTER FUNCTION connect_calibration_phmampl(integer, integer)
  OWNER TO postgres;

