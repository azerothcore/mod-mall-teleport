CREATE TABLE IF NOT EXISTS `premium` (
  `AccountId` int(11) unsigned NOT NULL,
  `active` int(11) unsigned NOT NULL DEFAULT '1',
  PRIMARY KEY (`AccountId`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
