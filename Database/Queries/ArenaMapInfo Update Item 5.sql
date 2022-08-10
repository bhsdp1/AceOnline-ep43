USE [atum2_db_account]
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER OFF
GO

ALTER TABLE [ti_ArenaMapInfo]
ADD LvDSupplyItemNum5 INT NULL,
LvDSupplyItemCount5 INT NULL
GO

USE [atum2_db_1]
GO
ALTER PROCEDURE [dbo].[atum_import_ti_arenamapinfo]
	@ArenaMapIndex			INT,
	@ArenaMode				TINYINT,
	@ReqMemberNum 			TINYINT,
	@PlayLimitedTime		TINYINT,
	@WINCondition			TINYINT,
	@LvDSupplyItemNum1		INT,
	@LvDSupplyItemCount1	INT,
	@LvDSupplyItemNum2		INT,
	@LvDSupplyItemCount2	INT,
	@LvDSupplyItemNum3		INT,
	@LvDSupplyItemCount3	INT,
	@LvDSupplyItemNum4		INT,
	@LvDSupplyItemCount4	INT,
	@LvDSupplyItemNum5		INT,
	@LvDSupplyItemCount5	INT
AS
	INSERT INTO atum2_db_account.dbo.ti_ArenaMapinfo(ArenaMapIndex, ArenaMode, ReqMemberNum , PlayLimitedTime, WINCondition, LvDSupplyItemNum1,
							 LvDSupplyItemCount1, LvDSupplyItemNum2, LvDSupplyItemCount2, LvDSupplyItemNum3, LvDSupplyItemCount3, LvDSupplyItemNum4, LvDSupplyItemCount4, LvDSupplyItemNum5, LvDSupplyItemCount5)
	VALUES (@ArenaMapIndex, @ArenaMode, @ReqMemberNum , @PlayLimitedTime, @WINCondition, @LvDSupplyItemNum1,
		@LvDSupplyItemCount1, @LvDSupplyItemNum2, @LvDSupplyItemCount2, @LvDSupplyItemNum3, @LvDSupplyItemCount3, @LvDSupplyItemNum4, @LvDSupplyItemCount4, @LvDSupplyItemNum5, @LvDSupplyItemCount5)
GO

ALTER PROCEDURE [dbo].[atum_LoadArenaMapInfo]
AS
	SELECT ArenaMapIndex, ArenaMode, ReqMemberNum, PlayLimitedTime, WINCondition, 
			LvDSupplyItemNum1, LvDSupplyItemCount1, LvDSupplyItemNum2, LvDSupplyItemCount2, LvDSupplyItemNum3, LvDSupplyItemCount3, LvDSupplyItemNum4, LvDSupplyItemCount4, LvDSupplyItemNum5, LvDSupplyItemCount5
	FROM atum2_db_account.dbo.ti_ArenaMapInfo WITH (NOLOCK)
GO

USE [atum2_db_arena]
GO
ALTER PROCEDURE [dbo].[atum_import_ti_arenamapinfo]
	@ArenaMapIndex			INT,
	@ArenaMode				TINYINT,
	@ReqMemberNum 			TINYINT,
	@PlayLimitedTime		TINYINT,
	@WINCondition			TINYINT,
	@LvDSupplyItemNum1		INT,
	@LvDSupplyItemCount1	INT,
	@LvDSupplyItemNum2		INT,
	@LvDSupplyItemCount2	INT,
	@LvDSupplyItemNum3		INT,
	@LvDSupplyItemCount3	INT,
	@LvDSupplyItemNum4		INT,
	@LvDSupplyItemCount4	INT,
	@LvDSupplyItemNum5		INT,
	@LvDSupplyItemCount5	INT
AS
	INSERT INTO atum2_db_account.dbo.ti_ArenaMapinfo(ArenaMapIndex, ArenaMode, ReqMemberNum , PlayLimitedTime, WINCondition, LvDSupplyItemNum1,
							 LvDSupplyItemCount1, LvDSupplyItemNum2, LvDSupplyItemCount2, LvDSupplyItemNum3, LvDSupplyItemCount3, LvDSupplyItemNum4, LvDSupplyItemCount4, LvDSupplyItemNum5, LvDSupplyItemCount5)
	VALUES (@ArenaMapIndex, @ArenaMode, @ReqMemberNum , @PlayLimitedTime, @WINCondition, @LvDSupplyItemNum1,
		@LvDSupplyItemCount1, @LvDSupplyItemNum2, @LvDSupplyItemCount2, @LvDSupplyItemNum3, @LvDSupplyItemCount3, @LvDSupplyItemNum4, @LvDSupplyItemCount4, @LvDSupplyItemNum5, @LvDSupplyItemCount5)
GO

ALTER PROCEDURE [dbo].[atum_LoadArenaMapInfo]
AS
	SELECT ArenaMapIndex, ArenaMode, ReqMemberNum, PlayLimitedTime, WINCondition, 
			LvDSupplyItemNum1, LvDSupplyItemCount1, LvDSupplyItemNum2, LvDSupplyItemCount2, LvDSupplyItemNum3, LvDSupplyItemCount3, LvDSupplyItemNum4, LvDSupplyItemCount4, LvDSupplyItemNum5, LvDSupplyItemCount5
	FROM atum2_db_account.dbo.ti_ArenaMapInfo WITH (NOLOCK)
GO