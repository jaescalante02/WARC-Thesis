-- segment_insert_WIT.sql
--
-- Procedimiento que incluye un segmento a la base de datos con la tecnica WIT.
--
-- Autores:
--
-- Samantha Campisi (samanthac07@gmail.com)
-- Juan A. Escalante (jaescalante02@gmail.com)

CREATE OR REPLACE PROCEDURE segment_insert_WIT(pid in number, url in varchar, 
                            cid in number, sid in number, filenamecdx in varchar,
                            filenamewarc in varchar)
is

        v_blob_cdx BLOB;
        v_bfile_cdx BFILE;
        v_blob_segment BLOB;
        v_bfile_segment BFILE;

BEGIN
        
        insert into WIT_SEGMENTO (S_ID, S_CAPTURA, S_SEGID, S_SEGMENTO)
        values (pid, cid, sid, EMPTY_BLOB()) returning S_SEGMENTO into v_blob_segment;

        v_bfile_segment:=BFILENAME('WARC_DIR', filenamewarc);
        DBMS_LOB.OPEN(v_bfile_segment, DBMS_LOB.LOB_READONLY);
        DBMS_LOB.LOADFROMFILE(v_blob_segment, v_bfile_segment, DBMS_LOB.GETLENGTH(v_bfile_segment));
        DBMS_LOB.CLOSE(v_bfile_segment);
        
        COMMIT;

END;

/
SHOW ERRORS PROCEDURE segment_insert_WIT;

