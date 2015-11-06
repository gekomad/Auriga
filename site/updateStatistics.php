<?php

function getUpdateTaskSQL($uuid_perft,$uuid_task){
return "update perft_tasks pt 
join 
	(select pt.uuid_perft, pt.uuid_task,  
		(select count(distinct engine)from tasks tt2 where tt2.uuid_task=t.uuid_task 
		  and tt2.uuid_perft='".$uuid_perft."' 
		  and tt2.uuid_task='".$uuid_task."'
		  group by tt2.uuid_task,tt2.uuid_perft )n_engine,  
		(select sum(tot)from tasks tt2 where tt2.uuid_task=t.uuid_task 
		  and tt2.uuid_perft='".$uuid_perft."' 
		  and tt2.uuid_task='".$uuid_task."'
		  group by tt2.uuid_task,tt2.uuid_perft )tot,  
     	  (select sum(minutes)from tasks tt2 where tt2.uuid_task=t.uuid_task 
		  and tt2.uuid_perft='".$uuid_perft."' 
		  and tt2.uuid_task='".$uuid_task."'
		  and tt2.tot is not null
		  group by tt2.uuid_task,tt2.uuid_perft )minutes,  
	ifnull(floor ((select count(distinct tt.fen) from tasks tt where tt.tot is not null and tt.uuid_task=t.uuid_task 
	and tt.uuid_perft='".$uuid_perft."'
	group by tt.uuid_perft,tt.uuid_task)/fens*100),0) perc_completed , 
	max(t.creation_date)creation_date from perft_tasks pt  
	left join tasks t  
	on  t.uuid_task=pt.uuid_task  
	where pt.uuid_perft ='".$uuid_perft."' 
	and pt.uuid_task ='".$uuid_task."' 
	group by pt.uuid_perft,pt.uuid_task  
	)t 
on t.uuid_perft=pt.uuid_perft and t.uuid_task=pt.uuid_task 
and pt.uuid_perft ='".$uuid_perft."' 
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
