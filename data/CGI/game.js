const targetNumber = Math.floor(Math.random() * 101);

let lives = 7;

const guessInput = document.getElementById('guessInput');
const guessButton = document.getElementById('guessButton');
const resetButton = document.getElementById('resetButton');
const message = document.getElementById('message');

function checkGuess() {
    const userGuess = parseInt(guessInput.value);
    
    if (isNaN(userGuess) || userGuess < 0 || userGuess > 100) {
        message.textContent = "Veuillez entrer un nombre valide entre 0 et 100.";
        return;
    }
    lives--;
    if (userGuess === targetNumber) {
        message.textContent = `Bravo ! Vous avez deviné le nombre ${targetNumber}.`;
        endGame();
    } else if (lives === 0) {
        message.textContent = `Vous avez épuisé toutes vos vies. Le nombre était ${targetNumber}.`;
        endGame();
    } else if (userGuess < targetNumber) {
        message.textContent = `Trop bas. Vous avez ${lives} vies restantes.`;
    } else {
        message.textContent = `Trop élevé. Vous avez ${lives} vies restantes.`;
    }
    guessInput.value = '';
}

function endGame() {
    guessButton.disabled = true;
    resetButton.style.display = 'inline';
}

resetButton.addEventListener('click', function() {
    location.reload();
});

guessButton.addEventListener('click', checkGuess);
