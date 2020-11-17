CREATE TABLE IF NOT EXISTS `premium` (
  `AccountId` int(11) unsigned NOT NULL,
  `active` tinyint(1) unsigned NOT NULL DEFAULT '1',
  PRIMARY KEY (`AccountId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
