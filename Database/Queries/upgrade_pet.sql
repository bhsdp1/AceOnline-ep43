USE [atum2_db_1]
GO

/****** Object:  StoredProcedure [dbo].[atum_Upgrade_Pet]    Script Date: 24.06.2015 22:31:48 ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO



CREATE PROCEDURE [dbo].[atum_Upgrade_Pet]
	@i_OldStoreUID				BIGINT,
	@i_NewItemNum				INT
AS
BEGIN
	SET NOCOUNT ON;

	DECLARE @insertedStoreUIDs	TABLE(Id BIGINT)
	DECLARE @newStoreUID		BIGINT

	-- Insert the new Pet
	INSERT INTO dbo.td_Store (AccountUniqueNumber, Possess, ItemStorage, Wear, CurrentCount, ItemWindowIndex, ItemNum, NumOfEnchants, PrefixCodeNum, SuffixCodeNum, CurrentEndurance, ColorCode, UsingTimeStamp, CreatedTime, ShapeItemNum, MainSvrItemUID, CoolingTime)
	OUTPUT INSERTED.UniqueNumber INTO @insertedStoreUIDs
	SELECT AccountUniqueNumber, Possess, ItemStorage, Wear, 1, 99, @i_NewItemNum, NumOfEnchants, PrefixCodeNum, SuffixCodeNum, CurrentEndurance, ColorCode, UsingTimeStamp, GETDATE(), ShapeItemNum, MainSvrItemUID, CoolingTime
	FROM dbo.td_Store
	WHERE UniqueNumber = @i_OldStoreUID

	SET @newStoreUID = (SELECT TOP 1 Id FROM @insertedStoreUIDs)

	-- Drop the old Pet
	DELETE FROM dbo.td_Store WHERE UniqueNumber = @i_OldStoreUID

	-- Update the Pet
	UPDATE td_pet
	SET
		ItemUID = @newStoreUID,
		PetIndex = (SELECT LinkItem FROM atum2_db_account.dbo.ti_Item WHERE ItemNum = @i_NewItemNum)
		-- Optional Resetting of the Pet Level
		--, [Level] = 1
		--, [Experience] = 0
	WHERE ItemUID = @i_OldStoreUID

	-- Return the new Store Item and the Pet Data
	SELECT 	  
	   s.[UniqueNumber]
      ,s.[AccountUniqueNumber]
      ,s.[Possess]
      ,s.[ItemStorage]
      ,s.[Wear]
      ,s.[CurrentCount]
      ,s.[ItemWindowIndex]
      ,s.[ItemNum]
      ,s.[NumOfEnchants]
      ,s.[PrefixCodeNum]
      ,s.[SuffixCodeNum]
      ,s.[CurrentEndurance]
      ,s.[ColorCode]
      ,s.[UsingTimeStamp]
      ,s.[CreatedTime]
      ,s.[ShapeItemNum]
      ,s.[MainSvrItemUID]
      ,s.[CoolingTime]
	  ,p.[ItemUID]
      ,p.[PetIndex]
      ,p.[Experience]
      ,p.[Level]
      ,p.[Name]
      ,p.[ExpRatio]
      ,p.[EnableSocketCount]
      ,p.[SocketItem1]
      ,p.[SocketItem2]
      ,p.[SocketItem3]
      ,p.[SocketItem4]
      ,p.[SocketItem5]
      ,p.[SocketItem6]
      ,p.[HPKitItemNum]
      ,p.[HPKitTriggerValue]
      ,p.[ShieldKitItemNum]
      ,p.[ShieldKitTriggerValue]
      ,p.[SPKitItemNum]
      ,p.[SPKitTriggerValue]
      ,p.[AutoSkilItemNum1]
      ,p.[AutoSkilItemNum2]
      ,p.[AutoSkilItemNum3]
      ,p.[AutoSkilItemNum4]
      ,p.[AutoSkilItemNum5]
      ,p.[AutoSkilItemNum6]
	FROM dbo.td_Store s
	INNER JOIN dbo.td_pet p ON p.ItemUID = s.UniqueNumber
	WHERE s.UniqueNumber = @newStoreUID
END


GO


