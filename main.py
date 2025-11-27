from fastapi import FastAPI, Request, Form
from fastapi.staticfiles import StaticFiles
import time

app = FastAPI(title="GEOScore", version="0.1.0")

# Монтируем статические файлы и шаблоны
app.mount("/", StaticFiles(directory="static", html=True), name="static")


@app.post("/analyze")
async def analyze_url(request: Request, url: str = Form(...)):
    # Имитируем обработку
    time.sleep(1)

    return { "id": 1 }

if __name__ == "__main__":
    import uvicorn
    uvicorn.run(app, host="0.0.0.0", port=8000)
