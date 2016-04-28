DROP TABLE "HVConfigEntry";
CREATE TABLE "HVConfigEntry"
(
  id serial NOT NULL,
  hvpm_id integer NOT NULL,
  hvconfig_id integer NOT NULL,
  voltageset real NOT NULL,
  CONSTRAINT "HVConfigEntry_pkey" PRIMARY KEY (id),
  CONSTRAINT "fk_hvconfigentrytohvpmconnection" FOREIGN KEY (hvpm_id)
      REFERENCES "HVPMConnection" (id) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION,
  CONSTRAINT "fkd8b58623f98aefd" FOREIGN KEY (hvconfig_id)
      REFERENCES "HVConfig" (id) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION
);
ALTER TABLE "HVConfigEntry"
  OWNER TO postgres;

