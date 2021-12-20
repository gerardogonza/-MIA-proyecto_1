create database if not exists practica1;
use practica1;
INSERT INTO locations ( location_type_code,location_type_name)  (SELECT location_type_code,location_type_name FROM t_locations);
INSERT INTO countrycodes (name,iso2,name_name,name_aiddata_code,name_aiddata_name,name_cow_alpha,name_cow_numeric,name_fao_code,name_fips,name_geonames_id,name_imf_code,name_iso2,name_iso3,name_iso_numeric,name_oecd_code,name_oecd_name,name_un_code,name_wb_code)  (SELECT name,iso2,name_name,name_aiddata_code,name_aiddata_name,name_cow_alpha,name_cow_numeric,name_fao_code,name_fips,name_geonames_id,name_imf_code,name_iso2,name_iso3,name_iso_numeric,name_oecd_code,name_oecd_name,name_un_code,name_wb_code FROM t_countrycodes);


insert into currency(name_Currency)  SELECT DISTINCT transaction_currency FROM t_transactions;
insert into status(name)  SELECT DISTINCT status FROM t_project;


INSERT INTO geoname(geoname.geoname_id,geoname.place_name,geoname.latitude,geoname.longitude,geoname.location_type_code,geoname.location_type_name,geoname.gazetteer_adm_code,geoname.gazetteer_adm_name,geoname.location_class,geoname.geographic_exactness)
 SELECT t_geoname.geoname_id,t_geoname.place_name,t_geoname.latitude,t_geoname.longitude,locations.idLocation,t_geoname.location_type_name,t_geoname.gazetteer_adm_code,t_geoname.gazetteer_adm_name,t_geoname.location_class,t_geoname.geographic_exactness
FROM locations,t_geoname
WHERE locations.location_type_code = t_geoname.location_type_code;


INSERT INTO project(project.project_id,project.is_geocoded,project.project_title ,project.start_actual_isodate,	project.end_actual_isodate ,	project.donors	,project.donors_iso3	,project.recipients	,project.recipients_iso3	,project.ad_sector_codes	,project.ad_sector_names	,project.status	,project.transactions_start_year	,project.transactions_end_year	,project.total_commitments,project.total_disbursements	)
SELECT DISTINCT t_project.project_id,t_project.is_geocoded,t_project.project_title,if(t_project.start_actual_isodate= '',NULL,STR_TO_DATE(t_project.start_actual_isodate, '%d/%m/%Y')),if(t_project.end_actual_isodate = '',NULL,STR_TO_DATE(t_project.end_actual_isodate, '%d/%m/%Y')),t_project.donors,t_project.donors_iso3,countrycodes.id,t_project.recipients_iso3,t_project.ad_sector_codes,t_project.ad_sector_names,status.idStatus,t_project.transactions_start_year,t_project.transactions_end_year,t_project.total_commitments,t_project.total_disbursements
FROM t_project INNER JOIN countrycodes ON (t_project.recipients = countrycodes.name OR t_project.recipients_iso3 = countrycodes.name_iso3) INNER JOIN status ON t_project.status = status.name;
DELETE t1 FROM project t1 INNER JOIN project t2 WHERE t1.idProyecto < t2.idProyecto AND t1.project_id = t2.project_id;


INSERT INTO transactions(transaction_id,project_id,transaction_isodate,transaction_year ,transaction_value_code,transaction_currency,transaction_value)
SELECT transaction_id,project.idProyecto,if(t_transactions.transaction_isodate = '', NULL, STR_TO_DATE(t_transactions.transaction_isodate, "%d/%m/%Y")),t_transactions.transaction_year,t_transactions.transaction_value_code,currency.idCurrency,t_transactions.transaction_value
FROM t_transactions, project, currency
WHERE project.project_id = t_transactions.project_id
AND t_transactions.project_id = project.project_id;


INSERT INTO level(project_id,project_location_id,geoname_id,transactions_start_year,transactions_end_year,even_split_commitments,even_split_disbursements )
SELECT project.idProyecto,t_level.project_location_id,geoname.id,t_level.transactions_start_year,t_level.transactions_end_year,t_level.even_split_commitments,t_level.even_split_disbursements
FROM project,t_level, geoname
WHERE project.project_id = t_level.project_id
AND geoname.geoname_id=t_level.geoname_id ;
DELETE t1 FROM level t1 INNER JOIN level t2 WHERE 1.id < t2.id AND t1.geoname_id = t2.geoname_id;