with x(WorkID, TaskID, Name, Deadline, Fulfillment, TimeSpent, cnt, len)
as
(
select WorkID, TaskID, Name, Deadline, FulfillmentDate, TimeSpent,
count(*) over(PARTITION by TaskID), 1
from WorksView 
where Date BETWEEN ':d1' and ':d2'
union ALL
select t1.WorkID, x.TaskID, x.Name, x.Deadline, x.Fulfillment, 
cast(x.TimeSpent || ';' || t1.TimeSpent as TEXT), x.cnt, x.len + 1
from WorksView t1 join x on x.TaskID = t1.TaskID and t1.WorkID > x.WorkID
where t1.Date BETWEEN ':d1' and ':d2'
)
select Name, Deadline, Fulfillment, TimeSpent
from x
where len=cnt
order by Name
