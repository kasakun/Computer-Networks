c = categorical({'Lookahead=1ms','Lookahead=2ms','Lookahead=3ms',...
    'Lookahead=4ms','Lookahead=5ms','Lookahead=10ms',...
    'Lookahead=15ms','Lookahead=20ms','Lookahead=25ms','Lookahead=50ms',...
    'Lookahead=100ms'});
bar(c,B);
%set(gca, 'XTickLabel',name);
ylabel('Time(s)')
legend('null','yawns');
title('WallClock Time - Lookahead Time');
