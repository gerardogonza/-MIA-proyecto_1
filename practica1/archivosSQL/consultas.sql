SELECT COUNT(*) FROM project WHERE YEAR(start_actual_isodate) = 2011;

SELECT idProyecto,DATEDIFF(end_actual_isodate,start_actual_isodate)as ProyectoMasDuracion,project_title FROM project ORDER BY 2 DESC LIMIT 1;

SELECT idProyecto,DATEDIFF(end_actual_isodate,start_actual_isodate)as 
ProyectoMenosDuracion FROM project  WHERE end_actual_isodate 
IS NOT NULL ORDER BY ProyectoMenosDuracion ASC LIMIT 1;

SELECT COUNT(recipients),countrycodes.name
FROM project, countrycodes WHERE project.recipients = countrycodes.id  GROUP BY countrycodes.name ORDER BY 1 DESC LIMIT 5;

SELECT COUNT(recipients),countrycodes.name
FROM project, countrycodes WHERE project.recipients = countrycodes.id  GROUP BY countrycodes.name ORDER BY 1 ASC LIMIT 5;

SELECT transaction_value,transaction_id,project_id,id
FROM transactions ORDER BY 1 DESC LIMIT 1;

SELECT transaction_value,transaction_id,project_id,id
FROM transactions ORDER BY 1 ASC LIMIT 1;

SELECT transaction_value,transaction_id,project_id,id
FROM transactions ORDER BY 1 DESC LIMIT 5;

SELECT SUM(transaction_value) FROM transactions;
	
SELECT transactions.id,project_title,transactions.transaction_value from project,transactions WHERE 
project_title LIKE "%RI-East Africa Public He%" AND transactions.project_id=project.idProyecto 
ORDER BY 2 DESC LIMIT 3
;





