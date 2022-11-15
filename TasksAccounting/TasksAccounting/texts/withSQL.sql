with x(WorkID, TaskID, Name, Content, Deadline, Fulfillment, TimeSpent, cnt, len)
as
(
select WorkID, TaskID, Name, Content, Deadline, FulfillmentDate, TimeSpent,
count(*) over(PARTITION by TaskID), 1
from WorksView 
where Date BETWEEN ':d1' and ':d2'
union ALL
select t1.WorkID, x.TaskID, x.Name, cast(x.Content || ';' || char(13) || t1.Content as TEXT), x.Deadline, x.Fulfillment,
cast(x.TimeSpent || ';' || t1.TimeSpent as TEXT),
x.cnt, x.len + 1
from WorksView t1 join x on x.TaskID = t1.TaskID and t1.WorkID > x.WorkID
where t1.Date BETWEEN ':d1' and ':d2'
)
select Name, Content, Deadline, Fulfillment, TimeSpent
from x
where len=cnt
order by Name
