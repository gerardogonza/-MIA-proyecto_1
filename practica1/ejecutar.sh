mysql -h localhost -u root -p < archivosSQL/drops.sql
mysql -h localhost -u root -p < archivosSQL/temporales.sql
mysql -h localhost -u root -p < archivosSQL/er.sql
mysql --local-infile=1 -u root -p < archivosSQL/controlgeonames.sql
mysql -h localhost -u root -p < archivosSQL/llenadoDB.sql
echo "hola"