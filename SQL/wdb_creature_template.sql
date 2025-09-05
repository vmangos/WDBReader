DROP TABLE IF EXISTS `wdb_creature_template`;
CREATE TABLE IF NOT EXISTS `wdb_creature_template` (
  `entry` int(10) unsigned NOT NULL,
  `build` smallint(5) unsigned NOT NULL,
  `name` varchar(128) COLLATE utf8_unicode_ci DEFAULT NULL,
  `name2` varchar(128) COLLATE utf8_unicode_ci DEFAULT NULL,
  `name3` varchar(128) COLLATE utf8_unicode_ci DEFAULT NULL,
  `name4` varchar(128) COLLATE utf8_unicode_ci DEFAULT NULL,
  `subname` varchar(128) COLLATE utf8_unicode_ci DEFAULT NULL,
  `type_flags` int(10) unsigned DEFAULT NULL,
  `static_flags1` int(10) unsigned DEFAULT NULL,
  `type` int(10) unsigned DEFAULT NULL,
  `pet_family` int(10) unsigned DEFAULT NULL,
  `rank` int(10) unsigned DEFAULT NULL,
  `unk1` int(10) unsigned DEFAULT NULL,
  `pet_spell_list_id` int(10) unsigned DEFAULT NULL,
  `display_id` int(10) unsigned DEFAULT NULL,
  `civilian` tinyint(3) unsigned DEFAULT NULL,
  `leader` tinyint(3) unsigned DEFAULT NULL,
  PRIMARY KEY (`entry`,`build`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
