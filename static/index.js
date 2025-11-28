const btn = document.getElementById('analyzeBtn');
const input = document.getElementById('url');
const progress = document.getElementById('progressBar');
const progressInner = document.getElementById('progressInner');
const results = document.getElementById('results');

// Функция проверки корректности URL
function isValidURL(url) {
    try {
      new URL(url);
      return true;
    } catch (_) {
      return false;
    }
}

function startAnalysis() {
const val = input.value.trim();
if (!val) {
  alert("Пожалуйста, введите URL.");
  return;
}

if (!isValidURL(val)) {
  alert("Некорректный URL. Проверьте формат (например, https://example.com).");
  return;
}

results.style.display = 'none';
progress.style.display = 'block';
progressInner.style.width = '0%';

let pct = 0;
const run = setInterval(() => {
  pct += 5;
  progressInner.style.width = pct + '%';
  if (pct >= 100) {
    clearInterval(run);
    results.style.display = 'grid';
  }
}, 120);
}

// Клик по кнопке
btn.addEventListener('click', startAnalysis);

// Нажатие Enter запускает анализ
input.addEventListener('keydown', (e) => {
if (e.key === 'Enter') {
  e.preventDefault(); // чтобы форма не отправлялась по умолчанию
  startAnalysis();
}
});
