A = [73.2978 73.2978; 76.5834 76.5834; 47.1692 47.1704; 45.59285 45.5948; 54.032325 54.0509; 53.738975 53.7461];
name = {'MPI=1 Sync=null','MPI=1 Sync=yawns','MPI=2 Sync=null','MPI=2 Sync=yawns','MPI=4 Sync=null','MPI=4 Sync=yawns',};
bar(A);
set(gca, 'XTickLabel',name);
ylabel('Time(s)')
legend('average','maximum');
title('WallClock Time - Mpi instances');