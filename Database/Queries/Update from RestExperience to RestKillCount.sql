USE atum2_db_account
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

-- Drop old Additional Experience
DROP TABLE dbo.td_AdditionalExperience
GO

-- Create Additional Kills
CREATE TABLE dbo.td_AdditionalKills(
	[AccountUniqueNumber] [int] NOT NULL,
	[RestKillCount] [int] NOT NULL,
PRIMARY KEY CLUSTERED 
(
	[AccountUniqueNumber] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON, FILLFACTOR = 90) ON [PRIMARY]
) ON [PRIMARY]
GO
ALTER TABLE [dbo].[td_AdditionalKills] ADD  CONSTRAINT [DF_td_AdditionalKills_RestKillCount]  DEFAULT ((3600)) FOR [RestKillCount]
GO

-- Insert Accounts
INSERT INTO dbo.td_AdditionalKills (AccountUniqueNumber)
SELECT AccountUniqueNumber FROM dbo.td_Account

-- Remove Procedures
USE atum2_db_1
GO
DROP PROCEDURE dbo.atum_Get_AddExp_RestExperienceCount
GO

USE atum2_db_1
GO
DROP PROCEDURE dbo.atum_Set_AddExp_RestExperienceCount
GO

-- Add New Procedures
USE atum2_db_1
GO
CREATE PROCEDURE dbo.atum_Get_AddKills_RestKillCount
	@i_AccountUniqueNumber		INT
AS
BEGIN
	SELECT RestKillCount FROM atum2_db_account.dbo.td_AdditionalKills WITH (NOLOCK) WHERE AccountUniqueNumber = @i_AccountUniqueNumber
END
GO

USE atum2_db_1
GO
CREATE PROCEDURE dbo.atum_Set_AddKills_RestKillCount
	@i_AccountUniqueNumber		INT,
	@i_RestKillCount			INT
AS
BEGIN
	DECLARE @RestKillCount	INT

	SET @RestKillCount = (SELECT RestKillCount FROM atum2_db_account.dbo.td_AdditionalKills WITH (NOLOCK) WHERE AccountUniqueNumber = @i_AccountUniqueNumber)

	IF( @RestKillCount IS NULL )
		BEGIN
			INSERT INTO [atum2_db_account].[dbo].td_AdditionalKills ([AccountUniqueNumber]) VALUES (@i_AccountUniqueNumber)
		END
	ELSE
		BEGIN
			UPDATE atum2_db_account.dbo.td_AdditionalKills SET RestKillCount = @i_RestKillCount WHERE AccountUniqueNumber = @i_AccountUniqueNumber
		END
END

