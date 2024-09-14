function Simplex()
    % Ввод размерности
    N = input('Введите размер задачи N: ');

    % Инициализация таблицы стоимости
    cost = zeros(N, N);
    disp('Введите элементы таблицы стоимости:');
    for i = 1:N
        row = input('', 's'); % Чтение строки в виде строки символов
        cost(i, :) = str2num(row); % Преобразование строки в массив чисел
    end

    solveAssignmentProblem(cost, N);
end

% Функция для вывода текущей симплекс-таблицы
function printTable(table, basis, N)
    disp('Симплекс-таблица:');
    fprintf('\t');
    for i = 1:N*N
        fprintf('x%d\t', i);
    end
    fprintf('\n');
    fprintf('z\t');
    p = 1;
    for i = 1:size(table, 1)
        fprintf('%f\t', table(i, :));
        fprintf('\n');
        if p <= length(basis) && basis(p) ~= -1
            fprintf('x%d\t', basis(p) + 1);
        else
            fprintf('\t');
        end
        p = p + 1;
    end
    fprintf('Базисные переменные: ');
    for i = 1:length(basis)
        if basis(i) ~= -1
            fprintf('x%d ', basis(i) + 1);
        end
    end
    fprintf('\n\n');
end

% Функция для нахождения разрешающего столбца
function pivotCol = findPivotColumn(table, basis, N)
    pivotCol = -1;
    maxdelta = 0;
    for p = 1:N*N
        delta = -table(1, p);
        for i = 1:length(basis)
            delta = delta + table(1, basis(i) + 1) * table(i + 1, p);
        end
        if delta > maxdelta
            maxdelta = delta;
            pivotCol = p;
        end
    end
end

% Функция для нахождения разрешающей строки
function pivotRow = findPivotRow(table, pivotCol, N)
    INF = 1e9;
    pivotRow = -1;
    minRatio = INF;
    for i = 2:2*N + 1
        if table(i, pivotCol) > 0
            ratio = table(i, end) / table(i, pivotCol);
            if ratio < minRatio
                minRatio = ratio;
                pivotRow = i;
            end
        end
    end
end

% Функция для выполнения симплекс-поворота
function [table,basis] = pivot(table, basis, pivotRow, pivotCol)
    pivotValue = table(pivotRow, pivotCol);

    % Делим строку разрешающего элемента на сам элемент
    table(pivotRow, :) = table(pivotRow, :) / pivotValue;

    % Обновляем остальные строки
    for i = 2:size(table, 1)
        if i ~= pivotRow
            factor = table(i, pivotCol);
            table(i, :) = table(i, :) - factor * table(pivotRow, :);
        end
    end

    % Обновляем базисные переменные
    basis(pivotRow-1) = pivotCol - 1;
end

% Основная функция для решения задачи о назначениях с использованием симплекс-метода
function solveAssignmentProblem(cost, N)
    % Инициализация симплекс-таблицы и базисных переменных
    table = zeros(2 * N + 1, N * N + 1);
    basis = -ones(1, 2 * N);  % Обратите внимание на правильную инициализацию размера массива

    % Заполняем строку целевой функции
    for j = 1:N*N
        table(1, j) = cost(floor((j - 1) / N) + 1, mod(j - 1, N) + 1);
    end

    % Заполняем таблицу ограничений
    for i = 2:N + 1
        for j = 1:N
            table(i, (i - 2) * N + j) = 1;
        end
        table(i, end) = 1;
    end
    for i = N + 2:2 * N + 1
        for j = 1:N*N
            table(i, j) = mod(j - 1, N) == mod(i - N - 2, N);
        end
        table(i, end) = 1;
    end

    % Заполнение массива basis
    for i = 1:N
        basis(i) = (i - 1) * N;
    end
    currentIndex = N + 1;
    for i = 1:N*N
        if isempty(find(basis(1:N) == i - 1, 1))
            basis(currentIndex) = i - 1;
            currentIndex = currentIndex + 1;
        end
        if currentIndex > 2 * N
            break;
        end
    end

    disp('Начальный базис построен');
    printTable(table, basis, N);

    % Симплекс-метод
    iteration = 1;
    while true
        pivotCol = findPivotColumn(table, basis, N);
        fprintf('Ведущий столбец: %f\n', pivotCol);
        if pivotCol < 0
            break;
        end
        pivotRow = findPivotRow(table, pivotCol, N);
        fprintf('Ведущая строка: %f\n', pivotCol);
        if pivotRow == -1
            disp('Решение не существует (неограниченная задача).');
            return;
        end

        [table, basis] = pivot(table, basis, pivotRow, pivotCol);
        fprintf('Итерация %d\n', iteration);
        iteration = iteration + 1;
        printTable(table, basis, N);
    end

    % Вывод оптимального решения
    totalSum = 0;
    disp('Оптимальное решение найдено:');
    for i = 1:length(basis)
        if table(i + 1, end) ~= 0
            col = mod(basis(i), N) + 1;
            row = floor(basis(i) / N) + 1;
            totalSum = totalSum + cost(row, col);
            fprintf('Работник %d назначен на задачу %d\n', row, col);
        end
    end
    fprintf('Сумма значений, соответствующих номерам в базисе: %f\n', totalSum);
end
