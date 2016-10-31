#! /usr/bin/awk -f
BEGIN{
	if(ARGC < 2){
		print "Usage:",ARGV[0],"reference_?.dlt" >> "/dev/stderr";
		exit(1);
	}#if
#初期値設定
i=0;
endi;
}#begin

#rangeの記録が0回の時
NF!=1 && dxtri[$2]==0{

dx[$2]=$3*$3;#そのrangeにdxdxをいれる
data[$2]=1.;#data?
dxtri[$2]=1;#triger+

range[i]=$2#range numberにrangeをいれる
i++;
endi=i-1;
}

#rangeの記録が1回以上あるとき
NF!=1 && dxtri[$2]==1{
dx[$2]+=$3*$3;
data[$2]+=1.;
}

END{
	if(ARGC < 2)exit(1);
	for(i=0;i<endi+1;i++){
		dx[range[i]]=sqrt(dx[range[i]]/data[range[i]]);
		print range[i],dx[range[i]];
	}
}#end
