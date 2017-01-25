#! /usr/bin/awk -f
BEGIN{
#how to
	if(ARGC < 2){
		print "Usage:",ARGV[0],"samlple*.dlt" >> "/dev/stderr";
		exit(1);
	}#if
#初期値設定
	i=0;
	cut=1000;
	print "imput sample cut number" >> "/dev/stderr";
	getline cut <"-";
}#begin

#all
{
	print ;
}

#sample数を逐次貯める
NF==1{
	i++;
}

#cut条件に達したら終了
NF==1 && i==cut{
	exit(1);
}

END{
	if(ARGC != 1){
		print "calc sample number:",i,"\tget sample number:",cut >> "/dev/stderr";
	}
}#end
