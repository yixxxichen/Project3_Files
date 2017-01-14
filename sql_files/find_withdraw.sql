delimiter $$
drop procedure if exists find_withdraw $$
create procedure find_withdraw(
	in sid char(20),
	in y char(20),
	in sem char(20)
)
begin
select t1.UoSCode,t1.UoSName from unitofstudy t1,transcript t2
where t2.Grade is NULL and t2.Year=y and t2.Semester=sem and t1.UoSCode=t2.UoSCode and t2.StudId=sid;
end $$
delimiter ;
call find_withdraw('3213','2016','Q1');