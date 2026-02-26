SELECT EXISTS(SELECT * FROM denormalized_checking_queue WHERE gv_name=$1 AND work_type=$2 AND old_work_number=$3) 
