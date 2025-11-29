const analyzeBtn = document.getElementById("analyzeBtn");
const urlInput = document.getElementById("url");

const progressBar = document.getElementById("progressBar");
const progressInner = document.getElementById("progressInner");
const compareSection = document.getElementById("compareSection");
const finalScore = document.getElementById("finalScore");
const LEFT_SCORES = [85, 88, 91, 79, 83, 88, 83, 89];
const RIGHT_SCORES = [90, 58, 89, 83, 88, 93, 87, 91];

// случайное число
function rnd(min = 40, max = 100) {
    return Math.floor(Math.random() * (max - min + 1) + min);
}

// приглушённые цвета
function levelColor(value) {
    if (value < 55) return "level-red";
    if (value < 75) return "level-yellow";
    return "level-green";
}

function animateProgress() {
    progressInner.style.width = "0";
    progressBar.style.display = "block";

    let width = 0;
    const timer = setInterval(() => {
        width += 3;
        progressInner.style.width = width + "%";

        if (width >= 100) clearInterval(timer);
    }, 30);
}

function fillBar(id, valId, value) {
    const bar = document.getElementById(id);
    const valueText = document.getElementById(valId);

    bar.className = "";
    bar.classList.add(levelColor(value));
    bar.style.width = value + "%";

    valueText.innerText = value;
}

analyzeBtn.addEventListener("click", () => {
    const url = urlInput.value.trim();
    if (!url) return alert("Введите URL");

    compareSection.style.display = "none";
    animateProgress();

    setTimeout(() => {
        progressBar.style.display = "none";

        const left = LEFT_SCORES; 
        const right = RIGHT_SCORES;

        // Итоговый рейтинг
        const score = Math.round(left.reduce((a, b) => a + b) / left.length);
        finalScore.innerText = score;
        finalScore.className = "final-score " + levelColor(score);

        // Шкалы (ваш сайт)
        left.forEach((val, i) => fillBar(`m${i+1}_l`, `m${i+1}_l_val`, val));

        // Шкалы (конкурент)
        right.forEach((val, i) => fillBar(`m${i+1}_r`, `m${i+1}_r_val`, val));

        compareSection.style.display = "block";

    }, 2000 + (1 + (600-1) * Math.random()));
});


function generateRecommendations(leftScores, rightScores) {
    // Устанавливаем максимальное количество карточек рекомендаций, которое будет показано.
    // Вы можете изменить это значение, например, на 5 или 2.
    const MAX_RECOMMENDATIONS = 3; 

    const topics = [
        "Логичность разметки",
        "Цитируемость контента",
        "Частота появления в выдаче",
        "Глубина контента",
        "Авторитет автора",
        "Фактическая точность",
        "Выделенность сущностей",
        "Актуальность контента"
    ];

    let recommendations = [];

    leftScores.forEach((val, i) => {
        const diff = rightScores[i] - val;
        
        // Собираем все метрики, где мы отстаем
        if (diff > 0) { 
            let idea = "";

            // Генерация примерных фраз-идей (SWITCH остается без изменений)
            switch (topics[i]) {
                case "Логичность разметки":
                    idea = "Проверьте использование семантических тегов (article, section, aside) и убедитесь в правильной вложенности заголовков (H1, H2...).";
                    break;
                case "Цитируемость контента":
                    idea = "Добавьте кнопки шеринга в социальных сетях и структурированные данные Schema.org для лучшего отображения в сниппетах.";
                    break;
                case "Частота появления в выдаче":
                    idea = "Оптимизируйте мета-теги Title и Description, чтобы они были более привлекательными и соответствовали поисковому интенту.";
                    break;
                case "Глубина контента":
                    idea = "Расширьте контент, добавив больше экспертных деталей, статистики или примеров. Минимум 2000 слов.";
                    break;
                case "Авторитет автора":
                    idea = "Добавьте блок 'Об авторе' с указанием его квалификации и ссылки на социальные сети (E-E-A-T).";
                    break;
                case "Фактическая точность":
                    idea = "Проверьте все данные и цитаты на наличие актуальных источников (ссылки на исследования, статистику).";
                    break;
                case "Выделенность сущностей":
                    idea = "Используйте жирный шрифт, списки и цитаты для выделения ключевых терминов и понятий (сущностей).";
                    break;
                case "Актуальность контента":
                    idea = "Проведите обновление страницы, заменив устаревшие данные и добавив свежую информацию. Укажите дату последнего обновления.";
                    break;
            }
            
            // Добавляем объект с данными и разницей для сортировки
            recommendations.push({
                title: topics[i],
                idea: idea,
                diff: diff 
            });
        }
    });

    if (recommendations.length === 0) {
        // Если нет отставания
        return [{ 
            title: "Отличная работа! 🏆", 
            idea: "Ваш сайт показывает превосходные результаты и не уступает ключевому конкуренту по всем метрикам. Продолжайте в том же духе!" 
        }];
    }
    
    // 1. Сортируем рекомендации по убыванию разницы (наибольшее отставание — в начале)
    recommendations.sort((a, b) => b.diff - a.diff);

    // 2. Ограничиваем количество рекомендаций
    recommendations = recommendations.slice(0, MAX_RECOMMENDATIONS);

    // 3. Форматируем заголовок после нарезки
    return recommendations.map(rec => ({
        title: `Усилить «${rec.title}» (Отставание: ${rec.diff} п.)`,
        idea: rec.idea
    }));
}
analyzeBtn.addEventListener("click", () => {
    const url = urlInput.value.trim();
    if (!url) return alert("Введите URL");

    compareSection.style.display = "none";
    recommendations.style.display = "none";
    animateProgress();

    setTimeout(() => {
        progressBar.style.display = "none";

        const left = LEFT_SCORES; 
        const right = RIGHT_SCORES;

        const score = Math.round(left.reduce((a, b) => a + b) / left.length);
        finalScore.innerText = score;
        finalScore.className = "final-score " + levelColor(score);

        left.forEach((val, i) => fillBar(`m${i+1}_l`, `m${i+1}_l_val`, val));
        right.forEach((val, i) => fillBar(`m${i+1}_r`, `m${i+1}_r_val`, val));

        compareSection.style.display = "block";

        // --- ОБНОВЛЁННЫЙ БЛОК РЕКОМЕНДАЦИЙ ---
        const recommendationsData = generateRecommendations(left, right);
        const recGrid = document.getElementById("recGrid");
        recGrid.innerHTML = ""; // Очищаем старые рекомендации

        recommendationsData.forEach(rec => {
            const card = document.createElement("div");
            card.classList.add("idea-card");
            card.innerHTML = `<h3>${rec.title}</h3><p>${rec.idea}</p>`;
            recGrid.appendChild(card);
        });

        recommendations.style.display = "block";

    }, 2000);
});