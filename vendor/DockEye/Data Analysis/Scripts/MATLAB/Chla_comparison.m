close all
clear all

% Load data
data = readtable('chla_data_11-10-25.xlsx');

% Convert first column to datetime
data.Date = datetime(data{:,1}, 'InputFormat', 'M-d-yy');  % adjust if needed

% Group and average
summary = groupsummary(data, 'Date', 'mean', {'DockEye', 'Horiba', 'EXO2'});
stdsum = groupsummary(data, 'Date', 'mean', {'DockEye', 'Horiba', 'EXO2'});

% Extract data for plotting
x = summary.mean_DockEye;
y = summary.mean_Horiba;
z = summary.mean_EXO2;
dates = summary.Date;

% Calculate standard deviation for the grouped data
stdSummary = groupsummary(data, 'Date', 'std', {'DockEye', 'Horiba', 'EXO2'});

xs = stdSummary.std_DockEye;
ys = stdSummary.std_Horiba;

% Extract x and y
x2 = data.DockEye;
y2 = data.Horiba;

% Linear regression
reg = fitlm(x2, y2);

% Get coefficients
coeffs = reg.Coefficients.Estimate;
slope = coeffs(2);
intercept = coeffs(1);
r2 = reg.Rsquared.Ordinary;

% Create fit line
xfit = linspace(min(x2), max(x2), 100);
yfit = slope * xfit + intercept;

% Plot
figure

%subplot(2,1,1)
%errorbar(dates, x, xs, 'o-', 'LineWidth', 1.5); hold on;
%errorbar(dates, y, ys, 's-', 'LineWidth', 1.5); hold on;
plot(dates, x, 'o-', 'LineWidth', 1.5); hold on;
plot(dates, y, 'o-', 'LineWidth', 1.5); hold on;
plot(dates, z, 'o-', 'LineWidth', 1.5); hold on;
xlabel('Date', 'FontSize', 20);
ylabel('Chlorophyll-a (µg/L)', 'FontSize', 20);
legend('DockEye (avg)', 'Horiba (avg)', 'EXO2', 'Location', 'best');
title('DockEye/Horiba/EXO2 Chl. a Comparison', 'FontSize', 20);
xtickangle(45);
set(gca, 'FontSize', 20); 
grid on;
box on;

% subplot(2,1,2)
% plot(x, y, 'wo', 'MarkerSize', 6, 'DisplayName', 'Data'); hold on;
% plot(xfit, yfit, 'b-', 'LineWidth', 2, 'DisplayName', 'Fit');
% xlabel('DockEye Chlorophyll-a (µg/L)', 'FontSize', 20);
% ylabel('Horiba Chlorophyll-a (µg/L)', 'FontSize', 20);
% title('DockEye/Horiba Regression', 'FontSize', 20);
% legend('Chlorophyll-a', 'Regression');
% set(gca, 'FontSize', 20); 
% grid on;
% box on;

% Add regression equation and R² to plot
% eqn = sprintf('y = %.3fx + %.3f\nR² = %.3f', slope, intercept, r2);
% text(-0.75, 7.35, eqn, 'FontSize', 18);

% Remove NaN values from y
valid_idx = ~isnan(y);  % Index of non-NaN values
x_valid = x(valid_idx);
y_valid = y(valid_idx);

% Spearman correlation
[rho, pval] = corr(x_valid, y_valid, 'Type', 'Spearman', 'Rows', 'complete');
[rho2, pval2] = corr(x, z, 'Type', 'Spearman');

% Display results
fprintf('DockEye/Horiba:');

fprintf('Spearman rho: %.3f\n', rho);
fprintf('Spearman p-value: %.9g\n', pval);

fprintf('DockEye/EXO2:');

fprintf('Spearman rho: %.3f\n', rho2);
fprintf('Spearman p-value: %.3g\n', pval2);

%%

% mask threshold
threshold_chlorophyll = 3;

% create mask
mask = (x_valid <= threshold_chlorophyll) & (y_valid <= threshold_chlorophyll);

% mask samples
x_filtered = x_valid(mask);
y_filtered = y_valid(mask);

% calculate Spearman on the filtered data
[rho_filtered, pval_filtered] = corr(x_filtered, y_filtered, 'Type', 'Spearman', 'Rows', 'complete');

fprintf('Filtered Spearman rho: %.4f\n', rho_filtered);
fprintf('Filtered Spearman p-value: %.4g\n', pval_filtered);