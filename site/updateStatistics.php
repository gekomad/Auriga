<?php

function getUpdateTaskSQL($uuid_task){
return "update perft_tasks pt 
join 
	(	
	select pt.uuid_perft, pt.uuid_task,  
		(select count(distinct engine)from tasks tt2 where tt2.uuid_task=t.uuid_task 
		  and tt2.uuid_task='".$uuid_task."'
		  group by tt2.uuid_task,tt2.uuid_perft )n_engine,  	
		  (select sum(d.tot) from ( select avg(tot*(select count(1) from task_fens tf2 where tf2.uuid_task='".$uuid_task."' and fen =tt2.fen
)) tot from tasks tt2 where
		   ifnull(tt2.tot,-1)!=-1 
		  and tt2.uuid_task='".$uuid_task."'
		  group by fen )d)tot,  
     	  (select sum(minutes)from tasks tt2 where tt2.uuid_task=t.uuid_task 
		  and tt2.uuid_task='".$uuid_task."'
		  and tt2.heartbeat =0
		  group by tt2.uuid_task,tt2.uuid_perft )minutes,  
	ifnull(floor ((select count(tas.fen) from tasks tas, task_fens tf2 where tas.heartbeat =0 and tas.fen=tf2.fen and tf2.id in(
	select distinct tf2.id from task_fens tf2 where tf2.uuid_task =t.uuid_task  and tf2.fen in (
select distinct f.fen from task_fens f
where f.uuid_task='".$uuid_task."' )))/fens*100),0) perc_completed , 
	max(t.creation_date)creation_date from perft_tasks pt  
	left join tasks t  
	on  t.uuid_task=pt.uuid_task  
	where pt.uuid_task ='".$uuid_task."' 
	group by pt.uuid_perft,pt.uuid_task  		
	)t 
on t.uuid_perft=pt.uuid_perft and t.uuid_task=pt.uuid_task 
and pt.uuid_task ='".$uuid_task."' 
set pt.engine =t.n_engine, pt.perc_completed = t.perc_completed,
pt.creation_date= t.creation_date,pt.tot= t.tot,
pt.hours= t.minutes/60";

}

function getUpdatePerftSQL($uuid_perft){ 
return "update perft p
join
	(select uuid_perft,floor(avg(ifnull(perc_completed,0)))perc_completed,max(creation_date)creation_date,sum(tot)tot,sum(hours)hours from perft_tasks pt 
	where pt.uuid_perft ='".$uuid_perft."' 
	group by uuid_perft)t
	on t.uuid_perft=p.uuid_perft
	set p.perc_completed=t.perc_completed,p.creation_date=t.creation_date,p.tot=t.tot,
	p.hours=t.hours";
}

?>
