clear;

%clf;
figure(1);
p2aN0 = importdata('p2a_N0.csv');
p2aN1 = importdata('p2a_N1.csv');
p2aN2 = importdata('p2a_N2.csv');
p2aN3 = importdata('p2a_N3.csv');


tag00 = zeros(size(p2aN0,1), 1);
for i = 2:size(p2aN0,1)
    if p2aN0(i,3) == 1 || ((p2aN0(i,3) - p2aN0(i-1,3)) ==42)
        tag00(i) = 0;
        continue;
    end
    for j = (i+1) :size(p2aN0,1)
        if p2aN0(j,3) == p2aN0(i,3)
            if(p2aN0(i-1,3) ~= p2aN0(i, 3))
                tag00(i) = 1;
            end
            tag00(j) = -20;
        end
    end
end

tag01 = zeros(size(p2aN1,1), 1);
for i = 2:size(p2aN1,1)
    if p2aN1(i,3) == 1 || ((p2aN1(i,3) - p2aN1(i-1,3)) ==42)
        tag01(i) = 0;
        continue;
    end
    for j = (i+1) :size(p2aN1,1)
        if p2aN1(j,3) == p2aN1(i,3)
            if(p2aN1(i-1,3) ~= p2aN1(i, 3))
                tag01(i) = 1;
            end
            tag01(j) = -20;
        end
    end
end

tag02 = zeros(size(p2aN2,1), 1);
for i = 2:size(p2aN2,1)
    if p2aN2(i,3) == 1||((p2aN2(i,3) - p2aN2(i-1,3)) ==42)
        tag02(i) = 0;
        continue;
    end
    for j = (i+1) :size(p2aN2,1)
        if p2aN2(j,3) == p2aN2(i,3)
            if(p2aN2(i-1,3) ~= p2aN2(i, 3))
                tag02(i) = 1;
            end
            tag02(j) = -20;
        end
    end
end

tag03 = zeros(size(p2aN3,1), 1);
for i = 2:size(p2aN3,1)
    if p2aN3(i,3) == 1 ||((p2aN3(i,3) - p2aN3(i-1,3)) ==42)
        tag03(i) = 0;
        continue;
    end
    for j = (i+1) :size(p2aN3,1)
        if p2aN3(j,3) == p2aN3(i,3)
            if(p2aN3(i-1,3) ~= p2aN3(i, 3))
                tag03(i) = 1;
            end
            tag03(j) = -20;
        end
    end
end


for i = 1:size(p2aN0,1)
    p2aN0m(i,2) = mod(p2aN0(i,2),90);
end
for i = 1:size(p2aN1,1)
    p2aN1m(i,2) = mod(p2aN1(i,2),90) + 100;
end
for i = 1:size(p2aN2,1)
    p2aN2m(i,2) = mod(p2aN2(i,2),90) + 200;
end
for i = 1:size(p2aN3,1)
    p2aN3m(i,2) = mod(p2aN3(i,2),90) + 300;
end
p2aN0m(:,1) = p2aN0(:,1);
p2aN1m(:,1) = p2aN1(:,1);
p2aN2m(:,1) = p2aN2(:,1);
p2aN3m(:,1) = p2aN3 (:,1);
scatter(p2aN0m(:,1),p2aN0m(:,2),'.');hold on;
scatter(p2aN1m(:,1),p2aN1m(:,2),'.');
scatter(p2aN2m(:,1),p2aN2m(:,2),'.');
scatter(p2aN3m(:,1),p2aN3m(:,2),'.');

%%
tag10 = mod(tag00.*p2aN0(:,2), 90);
for i = 1: size(p2aN0,1)
    if(tag00(i)  == 1)
        text(p2aN0(i,1), tag10(i),'X','FontSize',10);hold on;
        text(p2aN0(i,1), -20,'X','FontSize',10);hold on;
    end
    if(tag00(i)  == -20)
        text(p2aN0(i,1), tag10(i),'o','FontSize',4);hold on;
    end
end

tag11 = mod(tag01.*p2aN1(:,2), 90) + 100;
for i = 1: size(p2aN1,1)
    if(tag01(i)  == 1)
        text(p2aN1(i,1), tag11(i),'X','FontSize',10);hold on;
        text(p2aN1(i,1), -20,'X','FontSize',10);
    end
    if(tag01(i)  == -20)
        text(p2aN1(i,1), tag11(i),'o','FontSize',4);hold on;
    end
end

tag12 = mod(tag02.*p2aN2(:,2), 90) + 200;
for i = 1: size(p2aN2,1)
    if(tag02(i)  == 1)
        text(p2aN2(i,1), tag12(i),'X','FontSize',10);hold on;
         text(p2aN2(i,1), -20,'X','FontSize',10);hold on;
    end
    if(tag02(i)  == -20)
        text(p2aN2(i,1), tag12(i),'o','FontSize',4);hold on;
    end
end

tag13 = mod(tag03.*p2aN3(:,2), 90) + 300;
for i = 1: size(p2aN3,1)
    if(tag03(i)  == 1)
        text(p2aN3(i,1), tag13(i),'X','FontSize',10);hold on;
        text(p2aN3(i,1), -20,'X','FontSize',10);hold on;
    end
    if(tag03(i)  == -20)
        text(p2aN3(i,1), tag13(i),'o','FontSize',4);hold on;
    end
end
xlabel('Time');
ylabel('Packet Number (Mod 90) for Four Connections');
grid on;
