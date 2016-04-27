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

