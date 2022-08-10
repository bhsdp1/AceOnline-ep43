-- 2015-06-01 Future, Update Database to Shop Prices Per Building NPC
USE atum2_db_account
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER OFF
GO

create table ti_ShopNew(
	[ShopOrder] [int] NULL,
	[UniqueNumber] [int] NULL,
	[ItemNum] [int] NULL,
	[Price] [int] NOT NULL DEFAULT ((0))
) ON [PRIMARY]
GO



-- copy ti_Shop
insert into ti_ShopNew (ShopOrder, UniqueNumber, ItemNum)
select ShopOrder, UniqueNumber, ItemNum from ti_Shop
GO

-- merge item prices
UPDATE ti_ShopNew
SET ti_ShopNew.Price = i.Price
FROM ti_ShopNew s
INNER JOIN ti_Item i
ON s.ItemNum = i.ItemNum

-- merge cash or warpointshop prices
UPDATE ti_ShopNew
SET ti_ShopNew.Price = i.CashPrice
FROM ti_ShopNew s
INNER JOIN ti_Item i
ON i.ItemNum = s.ItemNum
WHERE s.UniqueNumber IN (SELECT BuildingIndex FROM ti_BuildingNPC where BuildingKind = 100 OR BuildingKind = 19)

GO



-- restructure ti_ItemInfo
alter table ti_ItemInfo
drop column CashPrice
exec sp_rename 'ti_ItemInfo.Price', 'SellingPrice', 'COLUMN'
GO

-- restructure ti_OverlapItem
alter table ti_OverlapItem
drop column CashPrice
GO

-- restructure ti_Shop
drop table ti_Shop
GO
exec sp_rename 'ti_ShopNew', 'ti_Shop'
GO

-- alter the ti_Item View
drop view dbo.ti_Item
GO

CREATE VIEW [dbo].[ti_Item] AS 
SELECT [dbo].[ti_ItemInfo].[ItemNum]
		  ,[dbo].[ti_ItemInfo].[Kind]
		  ,[dbo].[ti_ItemInfo].[ItemName]
		  ,[dbo].[ti_ItemInfo].[AbilityMin]
		  ,[dbo].[ti_ItemInfo].[AbilityMax]
		  ,[dbo].[ti_ItemInfo].[ReqRace]
		  ,[dbo].[ti_ItemInfo].[ReqAttackPart]
		  ,[dbo].[ti_ItemInfo].[ReqDefensePart]
		  ,[dbo].[ti_ItemInfo].[ReqFuelPart]
		  ,[dbo].[ti_ItemInfo].[ReqSoulPart]
		  ,[dbo].[ti_ItemInfo].[ReqShieldPart]
		  ,[dbo].[ti_ItemInfo].[ReqDodgePart]
		  ,[dbo].[ti_ItemInfo].[ReqUnitKind]
		  ,[dbo].[ti_ItemInfo].[ReqMinLevel]
		  ,[dbo].[ti_ItemInfo].[ReqMaxLevel]
		  ,[dbo].[ti_ItemInfo].[ReqItemKind]
		  ,[dbo].[ti_ItemInfo].[Weight]
		  ,[dbo].[ti_ItemInfo].[HitRate]
		  ,[dbo].[ti_ItemInfo].[Defense]
		  ,[dbo].[ti_ItemInfo].[FractionResistance]
		  ,[dbo].[ti_ItemInfo].[NaturalFaction]
		  ,[dbo].[ti_ItemInfo].[SpeedPenalty]
		  ,[dbo].[ti_ItemInfo].[Range]
		  ,[dbo].[ti_ItemInfo].[Position]
		  ,[dbo].[ti_ItemInfo].[Scarcity]
		  ,[dbo].[ti_ItemInfo].[Endurance]
		  ,[dbo].[ti_ItemInfo].[AbrasionRate]
		  ,[dbo].[ti_ItemInfo].[Charging]
		  ,[dbo].[ti_ItemInfo].[Luck]
		  ,[dbo].[ti_ItemInfo].[MinTradeQuantity]
		  ,[dbo].[ti_ItemInfo].[SellingPrice]
		  --,[dbo].[ti_ItemInfo].[CashPrice]
		  --, CASE 
		  --		WHEN [dbo].[ti_OverlapItem].[CashPrice] != -1 THEN [dbo].[ti_OverlapItem].[CashPrice]
		  --		ELSE [dbo].[ti_ItemInfo].[CashPrice]
		  --	END AS CashPrice
		  ,[dbo].[ti_ItemInfo].[DestParameter1]
		  ,[dbo].[ti_ItemInfo].[ParameterValue1]
		  ,[dbo].[ti_ItemInfo].[DestParameter2]
		  ,[dbo].[ti_ItemInfo].[ParameterValue2]
		  ,[dbo].[ti_ItemInfo].[DestParameter3]
		  ,[dbo].[ti_ItemInfo].[ParameterValue3]
		  ,[dbo].[ti_ItemInfo].[DestParameter4]
		  ,[dbo].[ti_ItemInfo].[ParameterValue4]
		  ,[dbo].[ti_ItemInfo].[DestParameter5]
		  ,[dbo].[ti_ItemInfo].[ParameterValue5]
		  ,[dbo].[ti_ItemInfo].[DestParameter6]
		  ,[dbo].[ti_ItemInfo].[ParameterValue6]
		  ,[dbo].[ti_ItemInfo].[DestParameter7]
		  ,[dbo].[ti_ItemInfo].[ParameterValue7]
		  ,[dbo].[ti_ItemInfo].[DestParameter8]
		  ,[dbo].[ti_ItemInfo].[ParameterValue8]
		  ,[dbo].[ti_ItemInfo].[ReAttacktime]
		  ,[dbo].[ti_ItemInfo].[Time]
		  ,[dbo].[ti_ItemInfo].[RepeatTime]
		  ,[dbo].[ti_ItemInfo].[Material]
		  ,[dbo].[ti_ItemInfo].[ReqMaterial]
		  ,[dbo].[ti_ItemInfo].[RangeAngle]
		  ,[dbo].[ti_ItemInfo].[UpgradeNum]
		  ,[dbo].[ti_ItemInfo].[LinkItem]
		  ,[dbo].[ti_ItemInfo].[MultiTarget]
		  ,[dbo].[ti_ItemInfo].[ExplosionRange]
		  ,[dbo].[ti_ItemInfo].[ReactionRange]
		  ,[dbo].[ti_ItemInfo].[ShotNum]
		  ,[dbo].[ti_ItemInfo].[MultiNum]
		  ,[dbo].[ti_ItemInfo].[AttackTime]
		  ,[dbo].[ti_ItemInfo].[ReqSP]
		  --,[dbo].[ti_ItemInfo].[SummonMonster]
		  , CASE 
				WHEN [dbo].[ti_OverlapItem].[Tab] != -1 THEN [dbo].[ti_OverlapItem].[Tab]
				ELSE [dbo].[ti_ItemInfo].[SummonMonster]
			END AS SummonMonster
		  ,[dbo].[ti_ItemInfo].[NextSkill]
		  ,[dbo].[ti_ItemInfo].[SkillLevel]
		  ,[dbo].[ti_ItemInfo].[SkillHitRate]
		  ,[dbo].[ti_ItemInfo].[SkillType]
		  ,[dbo].[ti_ItemInfo].[SkillTargetType]
		  ,[dbo].[ti_ItemInfo].[Caliber]
		  ,[dbo].[ti_ItemInfo].[OrbitType]
		  --,[dbo].[ti_ItemInfo].[ItemAttribute]
		  , CASE 
				WHEN [dbo].[ti_OverlapItem].[ItemAttribute] != -1 THEN [dbo].[ti_OverlapItem].[ItemAttribute]
				ELSE [dbo].[ti_ItemInfo].[ItemAttribute]
			END AS ItemAttribute      
		  ,[dbo].[ti_ItemInfo].[BoosterAngle]
		  ,[dbo].[ti_ItemInfo].[CameraPattern]
		  ,[dbo].[ti_ItemInfo].[SourceIndex]
		  ,[dbo].[ti_ItemInfo].[Description]
		  ,[dbo].[ti_ItemInfo].[EnchantCheckDestParam]
		  ,[dbo].[ti_ItemInfo].[InvokingDestParamID]
		  ,[dbo].[ti_ItemInfo].[InvokingDestParamIDByUse]
		FROM [dbo].[ti_ItemInfo] LEFT JOIN [dbo].[ti_OverlapItem] 
		ON [dbo].[ti_ItemInfo].[ItemNum] = [dbo].[ti_OverlapItem].[ItemNum]
GO

-- alter procedure for aquiring of the shop
USE atum2_db_1
GO
ALTER PROCEDURE dbo.atum_GetShopItems
	@BuildingIndex INT
AS
	SELECT i.ItemNum, i.ItemName, i.MinTradeQuantity, s.Price, i.Kind, i.SummonMonster, cs.CashShopBit, cs.RemainCountForLimitedEdition
	FROM atum2_db_account.dbo.ti_Item i WITH(NOLOCK) INNER JOIN atum2_db_account.dbo.ti_Shop s WITH(NOLOCK) ON i.ItemNum = s.ItemNum LEFT OUTER JOIN atum2_db_account.dbo.td_CashShop cs WITH(NOLOCK) ON s.ItemNum = cs.ItemNum
	WHERE @BuildingIndex = s.Uniquenumber
	ORDER BY s.ShopOrder

GO

-- alter overlap Item procedures
ALTER PROCEDURE [dbo].[atum_Insert_ti_OverlapItem]
	@ItemNum			INT,
	@Tab				INT,
	@ItemAttribute		INT
-- // 2009-02-17 by cmkwon, MARK for Yedang					-- // 2008-09-04 by cmkwon, ¿¹´ç º¸¾È°ü·Ã ¼û±ä °èÁ¤ Àû¿ë ÇÏ±â(SQL Server 2005ÀÌ»ó¸¸ Áö¿øÇÏ´Â ±â´É) - 
AS
	INSERT INTO atum2_db_account.dbo.ti_OverlapItem(ItemNum, Tab, ItemAttribute)
		VALUES (@ItemNum, @Tab, @ItemAttribute)

GO

ALTER PROCEDURE [dbo].[atum_Load_ti_OverlapItem]
AS
	SELECT ItemNum, Tab, ItemAttribute FROM atum2_db_account.dbo.ti_OverlapItem
GO