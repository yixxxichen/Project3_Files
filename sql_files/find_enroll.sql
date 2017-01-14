delimiter $$
drop procedure if exists find_enroll $$
create procedure find_enroll(
	in sid char(20),
	in y char(20),
	in sem char(20)
)
begin
select t1.UoSCode, t3.UoSName, t2.StudId,t4.PrereqUoSCode,t4.EnforcedSince,sum(t5.StudId),t5.Grade,t1.Enrollment,t1.MaxEnrollment,t7.Type, t6.ClassroomId
from (uosoffering t1 left outer join(select * from transcript where StudId = sid  ) as t2 on t1.UoSCode = t2.UoSCode) left outer join
(requires t4 left outer join(select * from transcript where StudId = sid  ) as t5 on t5.UoSCode = t4.PrereqUoSCode) on t1.UoSCode=t4.UoSCode,
unitofstudy t3,lecture t6,classroom t7
where t1.Year = y and
t1.Semester = sem and
t3.UoSCode = t1.UoSCode and
t6.UoSCode = t1.UoSCode and
t7.ClassroomId = t6.ClassroomId and 
t6.Semester = sem and
t6.Year = y
group by t1.UoSCode;
end $$
delimiter ;
#call find_enroll('3213','2016','Q1');
call find_enroll('3213','2017','Q2');