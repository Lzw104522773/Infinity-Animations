/* ======================================================================================================================================================================== *
 *  v 6.0.0                                                                                                                                                                 *
 *                                                 W e i D U    F J _ C R E _ V A L I D I T Y    P A T C H    R O U T I N E                                                 *
 *                                                                                                                                                                          *
 * ======================================================================================================================================================================== *
 *  Adapted from WeiDU FJ_CRE_VALIDITY PATCH function, this routine checks whether a CRE file is well-formed or not, fixes some common bugs, and reports whether the CRE    *
 *  file is still broken or not. In particular:                                                                                                                             *
 *    - reports invalidity if it is charbase.cre, shorter than the minimum size, the signature mismatches, or a sub-structure has members and its offset is in the header.  *
 *    - if a empty sub-structure�s offset is in the header, point them to the end of the header.                                                                            *
 *  RETURN                                                                                                                                                                  *
 *    - valid : 1 if the CRE is well-formed, 0 otherwise.                                                                                                                   *
 * ------------------------------------------------------------------------------------------------------------------------------------------------------------------------ *
 *  If the CRE is valid, we can call fj_cre_reindex.tpp to reorder creatures with nonstandard offset orders (known spells, spell memorization info, memorized spells,       *
 *  effects, items, item slot) and/or fj_cre_eff_v2.tpp to force the CRE file to use EFF V2 effects internally instead of EFF V1.                                           *
 * ======================================================================================================================================================================== */

tmp_0 = 0x2d4
tmp_1 = 0x2d4
tmp_2 = 0x2d4
tmp_3 = 0
SPRINT m1 @1201 // ~is corrupt~
SPRINT m2 @1202 // ~below minimum length~
SPRINT m3 @1203 // ~header misplaced~
SPRINT m4 @1204 // ~extended structures point to header~
SPRINT sg ~CRE V1.0~
valid = 1

PATCH_IF ~%SOURCE_RES%~ STRING_EQUAL_CASE charbase BEGIN

	valid = 0

END ELSE BEGIN

	PATCH_IF BUFFER_LENGTH < 0x2d4 BEGIN // PATCH_IF SOURCE_SIZE < 0x2d4 BEGIN

		valid = 0
		PATCH_PRINT ~%SOURCE_FILE% %m1%: %m2%.~ // is corrupt: below minimum length
		PATCH_SILENT

	END ELSE BEGIN

		READ_ASCII 0 sg
		PATCH_IF ~%sg%~ STR_CMP ~CRE V1.0~ BEGIN
			valid = 0
			PATCH_PRINT ~%SOURCE_FILE% %m1%: %m3%.~ // is corrupt: header misplaced
			PATCH_SILENT
		END ELSE BEGIN
			DEFINE_ASSOCIATIVE_ARRAY cre_offset BEGIN
				0x2a0 => 0x2a4
				0x2a8 => 0x2ac
				0x2b0 => 0x2b4
				0x2b8 => 0x2c0
				0x2bc => 0x2c0
				0x2c4 => 0x2c8
			END
			PHP_EACH cre_offset AS tmp => tmp_1 BEGIN
				READ_LONG tmp_0 tmp_2
				READ_LONG tmp_1 tmp_3
				PATCH_IF tmp_3 = 0 && tmp_2 < 0x2d4 BEGIN
					WRITE_LONG tmp_0 0x2d4
				END
				PATCH_IF tmp_3 != 0 && tmp_2 < 0x2d4 BEGIN
					valid = 0
					PATCH_PRINT ~%SOURCE_FILE% %m1%: %m4%.~ // is corrupt: extended structures point to header
					PATCH_SILENT
				END
			END
		END

	END

END
