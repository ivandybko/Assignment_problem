function Hungarian()
    N = input('Введите размер: '); 
    cost = zeros(N, N);
    disp('Введите элементы таблицы стоимости:');
    for i = 1:N
        row = input('', 's'); % Чтение строки в виде строки символов
        cost(i, :) = str2num(row); % Преобразование строки в массив чисел
    end
    result = hungarian(cost);
    disp(['Минимальная стоимость: ', num2str(result)]);
end

function cost = hungarian(a)
    n = size(a, 1);  
    u = zeros(1, n+1);  
    v = zeros(1, n+1);  
    p = zeros(1, n+1);  
    way = zeros(1, n+1); 

    for i = 1:n
        p(1) = i;  
        j0 = 1;  
        minv = inf(1, n+1);  
        used = false(1, n+1);  

        while true
            used(j0) = true;  
            i0 = p(j0);
            delta = inf;  
            j1 = 0;

            for j = 2:n+1
                if ~used(j)
                    cur = a(i0, j-1) - u(i0) - v(j);  
                    if cur < minv(j)  
                        minv(j) = cur;
                        way(j) = j0;
                    end
                    if minv(j) < delta  
                        delta = minv(j);
                        j1 = j;
                    end
                end
            end

            for j = 1:n+1
                if used(j)
                    u(p(j)) = u(p(j)) + delta;
                    v(j) = v(j) - delta;
                else
                    minv(j) = minv(j) - delta;
                end
            end

            j0 = j1;  

            if p(j0) == 0
                break;  
            end
        end

        while j0 ~= 1
            j1 = way(j0);
            p(j0) = p(j1);
            j0 = j1;
        end
    end

    ans = zeros(1, n+1);
    for j = 2:n+1
        ans(p(j)) = j-1;
    end

    cost = -v(1);  
end


