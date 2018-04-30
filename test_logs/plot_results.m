function plot_results(table, heading)
	% close all;
	% clear;
	% clc;

	% Plot X position
	figure();
	hold on;
	plot(table.time_step, table.kiv1_X, 'r')	
	range_x = [(table.kiv1_X_MAX - table.kiv1_X_AVG)' ; (table.kiv1_X_AVG - table.kiv1_X_MIN)'];
	shadedErrorBar(table.time_step, table.kiv1_X_AVG, range_x, 'lineprops', '--r')
	xlabel('Time')
	ylabel('X (m)')
	legend('X actual', 'X estimated')
    title([heading, ': X position'])
    grid on;
    saveas(gcf, ['figures/', heading, '_X_position.png'])

    % Plot Y position
    figure();
	hold on;
	plot(table.time_step, table.kiv1_Y, 'b')	
	range_y = [(table.kiv1_Y_MAX - table.kiv1_Y_AVG)' ; (table.kiv1_Y_AVG - table.kiv1_Y_MIN)'];
	shadedErrorBar(table.time_step, table.kiv1_Y_AVG, range_y, 'lineprops', '--b')
	xlabel('Time')
	ylabel('Y (m)')
	legend('Y actual', 'Y estimated')
    title([heading, ': Y position'])
    grid on
    saveas(gcf, ['figures/', heading, '_Y_position.png'])


    error_x = table.kiv1_X_AVG - table.kiv1_X;
    error_y = table.kiv1_Y_AVG - table.kiv1_Y;

    % Plot X error vs NUM_CONNECTED
	figure();
	scatter(table.kiv1_NUM_CONNECTED_GPSBOTS, error_x, 'r*')
	xlabel('Number of Connected GPS robots')
	ylabel('X error (m)')
    title([heading, ': X error'])
    grid on;
    saveas(gcf, ['figures/', heading, '_X_error.png'])

    % Plot X error vs NUM_CONNECTED
	figure();
	scatter(table.kiv1_NUM_CONNECTED_GPSBOTS, error_y, 'b*')
	xlabel('Number of Connected GPS robots')
	ylabel('Y error (m)')
    title([heading, ': Y error'])
    grid on;
    saveas(gcf, ['figures/', heading, '_Y_error.png'])

end
