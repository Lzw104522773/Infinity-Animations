/* ======================================================================================================================================== *
 *  v 6.0.0                                                                                                                                 *
 *                                   W e i D U    F J _ C R E _ E F F _ V 2    P A T C H    R O U T I N E                                   *
 *                                                                                                                                          *
 * ======================================================================================================================================== *
 *  Adapted from WeiDU FJ_CRE_VALIDITY PATCH function and usually called after FJ_CRE_VALIDITY or FJ_CRE_REINDEX patch, this routine force  *
 *  the CRE file to use EFF V2 effects internally instead of EFF V1 (i.e. converts creatures using version 1 effects to version 2).         *
 * ======================================================================================================================================== */

PATCH_IF fv != 1 BEGIN
	WRITE_BYTE 0x33 1
	fv = 1
	READ_LONG 0x2c8 fc      // Effects count
	PATCH_IF (fc > 0x0) BEGIN
		READ_LONG 0x2c4 fs  // Effects offset
		READ_ASCII fs ~fx~ (0x30 * fc)
		PATCH_FOR_EACH f1 IN 0x2a0 0x2a8 0x2b0 0x2b8 0x2bc BEGIN
			READ_LONG f1 f2
			WRITE_LONG f1 ((f2 > fs) ? (f2 + (fc * (0x108 - 0x30))) : (f2 < 0x2d4 ? 0x2d4 : f2))
		END
		DELETE_BYTES fs (0x30 * fc)
		INSERT_BYTES fs (0x108 * fc)
		SPRINT ~rfx~ ~~
		INNER_PATCH ~%fx%~ BEGIN
			FOR (i1 = 0 ; i1 < fc ; i1 += 1) BEGIN
				SOURCE_SIZE += 0xd8
				READ_SHORT ((i1 * 0x30) + 0x00) pc      // Opcode
				READ_BYTE  ((i1 * 0x30) + 0x02) tg      // Target
				READ_BYTE  ((i1 * 0x30) + 0x03) pw      // Power
				READ_LONG  ((i1 * 0x30) + 0x04) p1      // Parameter 1
				READ_LONG  ((i1 * 0x30) + 0x08) p2      // Parameter 2
				READ_BYTE  ((i1 * 0x30) + 0x0c) tm      // Timing mode
				READ_BYTE  ((i1 * 0x30) + 0x0d) dp      // Dispellability
				READ_LONG  ((i1 * 0x30) + 0x0e) dr      // Duration
				READ_BYTE  ((i1 * 0x30) + 0x12) b1      // Probability 1
				READ_BYTE  ((i1 * 0x30) + 0x13) b2      // Probability 2
				READ_ASCII ((i1 * 0x30) + 0x14) ~rf~    // ResRef
				READ_LONG  ((i1 * 0x30) + 0x1c) dt      // Dice thrown
				READ_LONG  ((i1 * 0x30) + 0x20) dz      // Die size
				READ_LONG  ((i1 * 0x30) + 0x24) st      // Save type
				READ_LONG  ((i1 * 0x30) + 0x28) sb      // Save bonus
				INNER_PATCH_SAVE ~rfx~ ~%rfx%~ BEGIN
					INSERT_BYTES ((i1 * 0x108) + 0x00) 0x110
					WRITE_LONG   ((i1 * 0x108) + 0x10) pc      // Opcode
					WRITE_LONG   ((i1 * 0x108) + 0x14) tg      // Target
					WRITE_LONG   ((i1 * 0x108) + 0x18) pw      // Power
					WRITE_LONG   ((i1 * 0x108) + 0x1c) p1      // Parameter 1
					WRITE_LONG   ((i1 * 0x108) + 0x20) p2      // Parameter 2
					WRITE_BYTE   ((i1 * 0x108) + 0x24) tm      // Timing mode
					WRITE_LONG   ((i1 * 0x108) + 0x28) dr      // Duration
					WRITE_SHORT  ((i1 * 0x108) + 0x2c) b1      // Probability 1
					WRITE_SHORT  ((i1 * 0x108) + 0x2e) b2      // Probability 2
					WRITE_ASCIIE ((i1 * 0x108) + 0x30) ~%rf%~  // ResRef
					WRITE_LONG   ((i1 * 0x108) + 0x38) dt      // Dice thrown
					WRITE_LONG   ((i1 * 0x108) + 0x3c) dz      // Die size
					WRITE_LONG   ((i1 * 0x108) + 0x40) st      // Save type
					WRITE_LONG   ((i1 * 0x108) + 0x44) sb      // Save bonus
					WRITE_BYTE   ((i1 * 0x108) + 0x5c) dp      // Dispellability
					WRITE_LONG   ((i1 * 0x108) + 0x80) (`0)
					WRITE_LONG   ((i1 * 0x108) + 0x84) (`0)
					WRITE_LONG   ((i1 * 0x108) + 0x88) (`0)
					WRITE_LONG   ((i1 * 0x108) + 0x8c) (`0)
					WRITE_LONG   ((i1 * 0x108) + 0xa4) (`0)
					DELETE_BYTES ((i1 * 0x108) + 0x08) 8
				END
			END
		END
		WRITE_ASCIIE fs ~%rfx%~
		pc = 0          // Opcode
		tg = 0          // Target
		pw = 0          // Power
		p1 = 0          // Parameter 1
		p2 = 0          // Parameter 2
		tm = 0          // Timing mode
		dp = 0          // Dispellability
		dr = 0          // Duration
		b1 = 100        // Probability 1
		b2 = 0          // Probability 2
		dt = 0          // Dice thrown
		dz = 0          // Die size
		st = 0          // Save type
		sb = 0          // Save bonus
		SPRINT ~rf~ ~~  // ResRef
	END
END
