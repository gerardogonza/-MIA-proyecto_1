create database if not exists practica1;
use practica1;
CREATE TABLE t_level(
         project_id VARCHAR(30) ,
         project_location_id VARCHAR(30),
         geoname_id	INT,
         transactions_start_year INT,
         transactions_end_year INT,
         even_split_commitments FLOAT,
         even_split_disbursements FLOAT
       
         );
         
CREATE TABLE t_transactions(
        transaction_id VARCHAR(40),
        project_id VARCHAR(80),
        transaction_isodate VARCHAR(30),
        transaction_year INT,
        transaction_value_code VARCHAR(20),
        transaction_currency VARCHAR(20),
        transaction_value FLOAT
       
    );


CREATE TABLE t_project(
        project_id	varchar(30),
        is_geocoded	INT,
        project_title VARCHAR(80),
        start_actual_isodate VARCHAR(30),	
        end_actual_isodate VARCHAR(30),	
        donors	VARCHAR(30),
        donors_iso3	VARCHAR(30),
        recipients	VARCHAR(30),
        recipients_iso3	VARCHAR(30),
        ad_sector_codes	VARCHAR(30),
        ad_sector_names	VARCHAR(30),
        status	VARCHAR(30),
        transactions_start_year	INT,
        transactions_end_year	INT,
        total_commitments	FLOAT,
        total_disbursements	FLOAT
         
        
);

    
CREATE TABLE t_geoname(
         geoname_id	INT,
         place_name	VARCHAR(30),
         latitude	FLOAT ,
         longitude	FLOAT ,
         location_type_code	VARCHAR(30),
         location_type_name	VARCHAR(80),
         gazetteer_adm_code	VARCHAR(70),
         gazetteer_adm_name	VARCHAR(70),
         location_class	INT,
         geographic_exactness INT
      
);

CREATE TABLE t_countrycodes (
    name VARCHAR(40), 
    iso2 VARCHAR(40),
    name_name VARCHAR(40),
    name_aiddata_code INT,
    name_aiddata_name VARCHAR(40),
    name_cow_alpha VARCHAR(40),
    name_cow_numeric INT,
    name_fao_code INT,
    name_fips VARCHAR(40),
    name_geonames_id INT,
    name_imf_code INT,
    name_iso2 VARCHAR(40),
    name_iso3 VARCHAR(40),
    name_iso_numeric INT,
    name_oecd_code INT,
    name_oecd_name VARCHAR(60),
    name_un_code INT,
    name_wb_code VARCHAR(40)

);
    
CREATE TABLE t_locations(
        location_type_code VARCHAR(30),
        location_type_name VARCHAR(70)
);


