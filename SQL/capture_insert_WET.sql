-- capture_insert_WET.sql
--
-- Procedimiento que incluye una captura en la base de datos con la técnica WET.
--
-- Autores:
--
-- Samantha Campisi (samanthac07@gmail.com)
-- Juan A. Escalante (jaescalante02@gmail.com)

CREATE OR REPLACE PROCEDURE capture_insert_WET(pid in number, url in varchar, 
                            cid in number, sid in number, filenamecdx in varchar,
                            filenamewarc in varchar)
    is

        v_blob_cdx BLOB;
        v_bfile_cdx BFILE;
        v_blob_segment BLOB;
        v_bfile_segment BFILE;

    BEGIN
        
        insert into WET_CAPTURA (C_ID, C_CAPTURA, C_FECHA, C_CDX)
        values (pid, cid, SYSDATE, EMPTY_BLOB()) returning C_CDX into v_blob_cdx;

        v_bfile_cdx:=BFILENAME('WARC_DIR', filenamecdx);
        DBMS_LOB.OPEN(v_bfile_cdx, DBMS_LOB.LOB_READONLY);
        DBMS_LOB.LOADFROMFILE(v_blob_cdx, v_bfile_cdx, DBMS_LOB.GETLENGTH(v_bfile_cdx));
        DBMS_LOB.CLOSE(v_bfile_cdx);
        
        insert into WET_SEGMENTO (S_ID, S_CAPTURA, S_SEGID, S_SEGMENTO)
        values (pid, cid, sid, EMPTY_BLOB()) returning S_SEGMENTO into v_blob_segment;

        v_bfile_segment:=BFILENAME('WARC_DIR', filenamewarc);
        DBMS_LOB.OPEN(v_bfile_segment, DBMS_LOB.LOB_READONLY);
        DBMS_LOB.LOADFROMFILE(v_blob_segment, v_bfile_segment, DBMS_LOB.GETLENGTH(v_bfile_segment));
        DBMS_LOB.CLOSE(v_bfile_segment);
        
        COMMIT;

    END;
    
/
SHOW ERRORS PROCEDURE capture_insert_WET;
