USE [atum2_db_account]
GO
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER OFF
GO

ALTER TABLE [dbo].[atum_backup_log_connection]
ADD [MACAddress] [varchar](24) NULL
GO

ALTER TABLE [dbo].[atum_log_connection]
ADD [MACAddress] [varchar](24) NULL
GO

USE [atum2_db_1]
GO

ALTER PROCEDURE [dbo].[atum_log_insert_connection]
	@i_LogType					TINYINT,
	@i_IPAddress				BINARY(4),
	@i_AccountUniqueNumber		INT,
	@i_AccountName				VARCHAR(20),
	@i_ServerName				VARCHAR(20),
	@i_ServerGroupID			INT,				-- 2006-09-14 by cmkwon, ÃƒÃŸÂ°Â¡Ã‡Ã”
	@i_PubilsherName			VARCHAR(20),		-- 2010-11 by dhjin, Â¾Ã†Â¶Ã³Â¸Â®Â¿Ã€ ÃƒÂ¤Â³ÃŽÂ¸Âµ Â·ÃŽÂ±Ã—Ã€ÃŽ.
	@i_MacAddress				VARCHAR(24)			-- 2015-09-23 Future, added logging of Mac Address
-- // 2009-02-17 by cmkwon, MARK for Yedang			-- // 2008-09-04 by cmkwon, Â¿Â¹Â´Ã§ ÂºÂ¸Â¾ÃˆÂ°Ã¼Â·Ãƒ Â¼Ã»Â±Ã¤ Â°Ã¨ÃÂ¤ Ã€Ã»Â¿Ã« Ã‡ÃÂ±Ã¢(SQL Server 2005Ã€ÃŒÂ»Ã³Â¸Â¸ ÃÃ¶Â¿Ã¸Ã‡ÃÂ´Ã‚ Â±Ã¢Â´Ã‰) - 
AS
	INSERT INTO atum2_db_account.dbo.atum_log_connection
	VALUES (@i_LogType, GetDate(), @i_IPAddress,
			@i_AccountUniqueNumber, @i_AccountName, @i_ServerName, @i_PubilsherName, @i_MacAddress)