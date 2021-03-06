-- createwot.sql
--
-- Archivo de creacion de las tablas necesarias para la tecnica WOT.
--
-- Autores:
--
-- Samantha Campisi (samanthac07@gmail.com)
-- Juan A. Escalante (jaescalante02@gmail.com)

  DROP TABLE WOT_SEGMENTO;
  DROP TABLE WOT_CAPTURA;
  DROP TABLE WOT_PAGINA;

  CREATE TABLE WOT_PAGINA (
       P_ID  NUMBER(8) NOT NULL,  
       P_URL    VARCHAR(512) NOT NULL,
       CONSTRAINT PK_WOT_PAGINA PRIMARY KEY (P_ID)
  ) TABLESPACE TS_WOT;

  CREATE TABLE WOT_CAPTURA (
       C_ID  NUMBER(8) NOT NULL,       
       C_CAPTURA   NUMBER (16) NOT NULL,
       C_FECHA DATE,
       C_CDX BLOB default empty_blob(),
       CONSTRAINT PK_WOT_CAPTURA PRIMARY KEY (C_ID, C_CAPTURA),
       CONSTRAINT FK_WOT_CAP_PAG FOREIGN KEY (C_ID) REFERENCES WOT_PAGINA
  ) TABLESPACE TS_WOT;   

  CREATE TABLE WOT_SEGMENTO (
       S_ID  NUMBER(8) NOT NULL,
       S_CAPTURA   NUMBER (16) NOT NULL,
       S_SEGID     NUMBER (8) NOT NULL,
       S_SEGMENTO BLOB default empty_blob(),
       CONSTRAINT PK_WOT_SEGMENTO PRIMARY KEY (S_ID, S_CAPTURA, S_SEGID),
       CONSTRAINT FK_WOT_SEG_PAG FOREIGN KEY (S_ID) REFERENCES WOT_PAGINA,       
       CONSTRAINT FK_WOT_SEG_CAP FOREIGN KEY (S_ID, S_CAPTURA) REFERENCES WOT_CAPTURA
  ) TABLESPACE TS_WOT;

  ALTER SESSION SET EVENTS= 'immediate trace name flush_cache';
  EXIT;

