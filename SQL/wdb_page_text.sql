DROP TABLE IF EXISTS `wdb_page_text`;
CREATE TABLE IF NOT EXISTS `wdb_page_text` (
  `entry` mediumint(8) unsigned NOT NULL,
  `build` smallint(5) unsigned NOT NULL,
  `next_page` mediumint(8) unsigned DEFAULT NULL,
  `text` longtext,
  PRIMARY KEY (`entry`,`build`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Item System';
