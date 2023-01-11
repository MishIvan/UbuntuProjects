with recursive x
as
(
select workid, taskid, name, content, deadline, fulfillmentdate, cast(timespent as text) timespent,
count(*) over(PARTITION by taskid) cnt , 1 len
from public.worksview
where date BETWEEN ':d1' and ':d2'
union ALL
select t1.workid, x.taskid, x.name, concat(x.content, ';', chr(13) ,t1.content), x.deadline, x.fulfillmentdate,
cast(concat(x.timespent, ';' ,t1.timespent) as text),
x.cnt, x.len + 1
from public.worksview t1 join x on x.taskid = t1.taskid and t1.workid > x.workid
where t1.date BETWEEN ':d1' and ':d2'
)
select name, content, deadline, fulfillmentdate, timespent
from x
where len=cnt
order by name;