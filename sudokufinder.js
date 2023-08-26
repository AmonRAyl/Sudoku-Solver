const apiUrl = 'https://sudoku-api.vercel.app/api/dosuku';
const fs = require('fs');

function fetchSudoku() {
  fetch(apiUrl)
    .then(response => {
      if (response.ok) {
        return response.json();
      } else {
        throw new Error('HTTP request failed: ' + response.status);
      }
    })
    .then(data => {
      let aux=data["newboard"]["grids"][0];
        let coordinates = [];
          for (let i = 0; i < 9; i++) {
              for (let j = 0; j < 9; j++){
                  if (aux["value"][i][j] != 0){
                      let string= "g["+i+"]["+j+"].setActualValue('"+aux["value"][i][j]+"',0);";
                      coordinates.push(string);
                  }
              }
          }

        let newContent = `Difficulty:\n${aux["difficulty"]}\nProblem:\n${matrixToString(aux["value"])}\nSolution:\n${matrixToString(aux["solution"])}\n\n${coordinates.join('\n')}`;
        
        function matrixToString(matrix) {
          return matrix.map(row => row.join(' ')).join('\n');
        }
        
        fs.appendFile('sudokus.txt', '\n\n' + newContent, (err) => {
          if (err) {
            console.error('Error appending to file:', err);
          } else {
            console.log('New data appended to output.txt');
          }
        });
    })
    .catch(error => {
      console.error('Error:', error);
    });
}

fetchSudoku();
