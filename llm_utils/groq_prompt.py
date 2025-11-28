import requests
from logic import HtmlParser


def prompt_groq(api_key: str, meta: HtmlParser):
    headers = {
        "Authorization": f"Bearer {api_key}",
        "Content-Type": "application/json",
    }

    prompt = f"""
        You are an expert in GEO (Generative Engine Optimization).

        Analyze a website and estimate how well LLMs will index it.

        HEADERS:
        {meta.headers}

        TITLES:
        {meta.title}

        AUTHORS:
        {meta.author}

        DESCRIPTION:
        {meta.description}
        
        KEYWORDS:
        {meta.keywords}
        
        LINES:
        {meta.lines}
    
        Provide:
        1. LLM visibility score (0–10)
        2. Main structural issues
        3. Missing semantic blocks
        4. How to improve meta-tags
        5. How to improve headers (h1–h3)
        6. How to improve content for generative search
        7. Specific examples of rewritten fragments
        8. Estimated snippet-likelihood (0–10)
        """

    json_data = {
        "model": "llama-3.3-70b-versatile",  # или другая модель, доступная в GroqCloud
        "messages": [
            {"role": "system", "content": "You are a helpful assistant."},
            {"role": "user", "content": prompt}
        ],
        "temperature": 0.7,
        "max_tokens": 512
    }

    response = requests.post(
        "https://api.groq.com/openai/v1/chat/completions",
        headers=headers,
        json=json_data
    )

    return response.json()