
const wordInput = document.getElementById("wordInput");
const splitButton = document.getElementById("splitButton");
const result = document.getElementById("result");

splitButton.addEventListener("click", async () => {
    const word = wordInput.value.trim();

    if (!word) {
        result.textContent = "कृपया कोई शब्द लिखें।";
        return;
    }

    result.textContent = "Processing...";

    try {
        const response = await fetch(
            `/split?word=${encodeURIComponent(word)}`
        );

        const data = await response.text();
        result.textContent = data;
    } catch (error) {
        result.textContent = "Server से संपर्क नहीं हो पाया।";
    }
});
