%% clear;

%clf;
figure(1);
p1 = importdata('./flow0.csv');
p2 = importdata('./flow1.csv');
p3 = importdata('./flow2.csv');
p4 = importdata('./flow3.csv');
p5 = importdata('./flow4.csv');
p6 = importdata('./flow5.csv');
p7 = importdata('./flow6.csv');
p8 = importdata('./flow7.csv');
p9 = importdata('./flow8.csv');
p10 = importdata('./flow9.csv');
p11 = importdata('./flow10.csv');
p12 = importdata('./flow11.csv');
p13 = importdata('./flow12.csv');
p14 = importdata('./flow13.csv');
p15 = importdata('./flow14.csv');
p16 = importdata('./flow15.csv');
p17 = importdata('./flow16.csv');
p18 = importdata('./flow17.csv');
p19 = importdata('./flow18.csv');
p20 = importdata('./flow19.csv');
p21 = importdata('./flow20.csv');
p22 = importdata('./flow21.csv');
p23 = importdata('./flow22.csv');
p24 = importdata('./flow23.csv');
p25 = importdata('./flow24.csv');
p26 = importdata('./flow25.csv');
p27 = importdata('./flow26.csv');
p28 = importdata('./flow27.csv');
p29 = importdata('./flow28.csv');
p30 = importdata('./flow29.csv');
p31 = importdata('./flow30.csv');
p32 = importdata('./flow31.csv');
p33 = importdata('./flow32.csv');
p34 = importdata('./flow33.csv');
p35 = importdata('./flow34.csv');
p36 = importdata('./flow35.csv');
p37 = importdata('./flow36.csv');
p38 = importdata('./flow37.csv');
p39 = importdata('./flow38.csv');
p40 = importdata('./flow39.csv');
p41 = importdata('./flow40.csv');
%%
tag00 = zeros(size(p1,1), 1);
for i = 2:size(p1,1)
    if p1(i,3) == 1 || ((p1(i,3) - p1(i-1,3)) ==42 || tag00(i) == -20)
        continue;
    end
    for j = (i+1) :size(p1,1) 
        if p1(j,3) == p1(i,3)
            tag00(j) = -20;    
            tag00(i) = 1;
        end   
    end
end
% 
tag01 = zeros(size(p27,1), 1);
for i = 2:size(p27,1)
    if p27(i,3) == 1 || ((p27(i,3) - p27(i-1,3)) ==42 || tag01(i) == -20)
        continue;
    end
    for j = (i+1) :size(p27,1) 
        if p27(j,3) == p27(i,3)
            tag01(j) = -100;    
            tag01(i) = 1;
        end   
    end
end

tag02 = zeros(size(p38,1), 1);
for i = 2:size(p38,1)
    if p38(i,3) == 1 || ((p38(i,3) - p38(i-1,3)) ==42 || tag02(i) == -20)
        continue;
    end
    for j = (i+1) :size(p38,1) 
        if p38(j,3) == p38(i,3)
            tag02(j) = -100;    
            tag02(i) = 1;
        end   
    end
end

tag03 = zeros(size(p19,1), 1);
for i = 2:size(p19,1)
    if p19(i,3) == 1 || ((p19(i,3) - p19(i-1,3)) ==42 || tag03(i) == -20)
        continue;
    end
    for j = (i+1) :size(p19,1) 
        if p19(j,3) == p19(i,3)
            tag03(j) = -100;    
            tag03(i) = 1;
        end   
    end
end


%%
scatter(p1(:,1),p1(:,2),'.');hold on;
scatter(p2(:,1),p2(:,2),'.');
scatter(p3(:,1),p3(:,2),'.');
scatter(p4(:,1),p4(:,2),'.');
scatter(p5(:,1),p5(:,2),'.');
scatter(p6(:,1),p6(:,2),'.');
scatter(p7(:,1),p7(:,2),'.');
scatter(p8(:,1),p8(:,2),'.');
scatter(p9(:,1),p9(:,2),'.');
scatter(p10(:,1),p10(:,2),'.');
scatter(p11(:,1),p11(:,2),'.');
scatter(p12(:,1),p12(:,2),'.');
scatter(p13(:,1),p13(:,2),'.');
scatter(p14(:,1),p14(:,2),'.');
scatter(p15(:,1),p15(:,2),'.');
scatter(p16(:,1),p16(:,2),'.');
scatter(p17(:,1),p17(:,2),'.');
scatter(p18(:,1),p18(:,2),'.');
scatter(p19(:,1),p19(:,2),'.');
scatter(p20(:,1),p20(:,2),'.');
scatter(p21(:,1),p21(:,2),'.');
scatter(p22(:,1),p22(:,2),'.');
scatter(p23(:,1),p23(:,2),'.');
scatter(p24(:,1),p24(:,2),'.');
scatter(p25(:,1),p25(:,2),'.');
scatter(p26(:,1),p26(:,2),'.');
scatter(p27(:,1),p27(:,2),'.');
scatter(p28(:,1),p28(:,2),'.');
scatter(p29(:,1),p29(:,2),'.');
scatter(p30(:,1),p30(:,2),'.');
scatter(p31(:,1),p31(:,2),'.');
scatter(p32(:,1),p32(:,2),'.');
scatter(p33(:,1),p33(:,2),'.');
scatter(p34(:,1),p34(:,2),'.');
scatter(p35(:,1),p35(:,2),'.');
scatter(p36(:,1),p36(:,2),'.');
scatter(p37(:,1),p37(:,2),'.');
scatter(p38(:,1),p38(:,2),'.');
scatter(p39(:,1),p39(:,2),'.');
scatter(p40(:,1),p40(:,2),'.');
scatter(p41(:,1),p41(:,2),'.');
%%
for i = 1:2: size(p1,1)
    if(tag00(i)  == 1)
        text(p1(i,1), p1(i,2).*tag00(i),'X','FontSize',10);hold on;
        text(p1(i,1), -100,'X','FontSize',10);hold on;
    end
end
% 
for i = 1:15: size(p27,1)
    if(tag01(i)  == 1)
        text(p27(i,1), p27(i,2).*tag01(i),'X','FontSize',10);hold on;
        text(p27(i,1), -100,'X','FontSize',10);hold on;
    end
end  

for i = 1:25: size(p38,1)
    if(tag02(i)  == 1)
        text(p38(i,1), p38(i,2).*tag02(i),'X','FontSize',10);hold on;
        text(p38(i,1), -100,'X','FontSize',10);hold on;
    end
end  

for i = 1:30: 500
    if(tag03(i)  == 1)
        text(p19(i,1), p19(i,2).*tag03(i),'X','FontSize',10);hold on;
        text(p19(i,1), -100,'X','FontSize',10);hold on;
    end
end  


xlabel('Time');
ylabel('Packet Number Each Connection');
grid on;
