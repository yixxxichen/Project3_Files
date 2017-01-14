delimiter $$
drop procedure if exists enroll_class $$
create procedure enroll_class(
	in sid char(20),
    in cid char(20),
	in y char(20),
	in sem char(20)
)
begin
insert into transcript values(sid,cid,sem,y,null);
update uosoffering set Enrollment = Enrollment +1 where UoSCode = cid and Semester=sem and Year=y;
end $$
delimiter ;
###call enroll_class('3213','COMP5348','2016','Q1');
###delete from transcript where StudId='3213' and UoSCode='COMP2129';
###update uosoffering set Enrollment = Enrollment -1 where UoSCode = 'COMP5348' and Semester='Q1' and Year='2016';