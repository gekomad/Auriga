
function isNumeric(n) {
  return !isNaN(parseFloat(n)) && isFinite(n);
}

function validate_single_fen() {
	res=validate_fen1();

	if(res.valid==false){
		fen =document.getElementById('fen');
		fen.style.backgroundColor = "red";
		document.getElementById("error_fen").className = 'nohidden';
		return false;
	}
	fen =document.getElementById('fen');
	fen.style.backgroundColor = "initial";
	document.getElementById("error_fen").className = 'hidden';
}
function validate() {
	ff=validate_single_fen();
	if(ff==false)return;
	

	depth =document.getElementById('depth');
	if(!isNumeric(depth.value)|| (depth.value<2||depth.value>100)){		
		depth.style.backgroundColor = "red";
		document.getElementById("error_depth").className = 'nohidden';
		return false;
	}
	document.getElementById("error_depth").className = 'hidden';
	depth.style.backgroundColor ="initial";

	tasks =document.getElementById('tasks');
	if(!isNumeric(tasks.value)|| (tasks.value<1||tasks.value>1000)){
		tasks.style.backgroundColor = "red";
		document.getElementById("error_tasks").className = 'nohidden';
		return false;
	}
	document.getElementById("error_tasks").className = 'hidden';
	tasks.style.backgroundColor ="initial";

	cap =document.getElementById('captcha');
	if(cap.value.trim()==""){
		cap.style.backgroundColor = "red";
		return false;
	}
	cap.style.backgroundColor ="initial";
	
	

	return true;	
 }


 function validate_fen(fen) {

	//https://github.com/jhlywa/chess.js
    var errors = {
       0: 'No errors.',
       1: 'FEN string must contain six space-delimited fields.',
       2: '6th field (move number) must be a positive integer.',
       3: '5th field (half move counter) must be a non-negative integer.',
       4: '4th field (en-passant square) is invalid.',
       5: '3rd field (castling availability) is invalid.',
       6: '2nd field (side to move) is invalid.',
       7: '1st field (piece positions) does not contain 8 \'/\'-delimited rows.',
       8: '1st field (piece positions) is invalid [consecutive numbers].',
       9: '1st field (piece positions) is invalid [invalid piece].',
      10: '1st field (piece positions) is invalid [row too large].',
    };

    /* 1st criterion: 6 space-seperated fields? */
    var tokens = fen.split(/\s+/);

	if (tokens.length == 4) {
      fen+=" 0 1";
    }
	if (tokens.length == 3) {
      fen+=" - 0 1";
    }

    var tokens = fen.split(/\s+/);
    if (tokens.length !== 6) {
      return {valid: false, error_number: 1, error: errors[1]};
    }

    /* 2nd criterion: move number field is a integer value > 0? */
    if (isNaN(tokens[5]) || (parseInt(tokens[5], 10) <= 0)) {
      return {valid: false, error_number: 2, error: errors[2]};
    }

    /* 3rd criterion: half move counter is an integer >= 0? */
    if (isNaN(tokens[4]) || (parseInt(tokens[4], 10) < 0)) {
      return {valid: false, error_number: 3, error: errors[3]};
    }

    /* 4th criterion: 4th field is a valid e.p.-string? */
    if (!/^(-|[abcdefgh][36])$/.test(tokens[3])) {
      return {valid: false, error_number: 4, error: errors[4]};
    }

    /* 5th criterion: 3th field is a valid castle-string? */
    if( !/^(KQ?k?q?|Qk?q?|kq?|q|-)$/.test(tokens[2])) {
      return {valid: false, error_number: 5, error: errors[5]};
    }

    /* 6th criterion: 2nd field is "w" (white) or "b" (black)? */
    if (!/^(w|b)$/.test(tokens[1])) {
      return {valid: false, error_number: 6, error: errors[6]};
    }

    /* 7th criterion: 1st field contains 8 rows? */
    var rows = tokens[0].split('/');
    if (rows.length !== 8) {
      return {valid: false, error_number: 7, error: errors[7]};
    }

    /* 8th criterion: every row is valid? */
    for (var i = 0; i < rows.length; i++) {
      /* check for right sum of fields AND not two numbers in succession */
      var sum_fields = 0;
      var previous_was_number = false;

      for (var k = 0; k < rows[i].length; k++) {
        if (!isNaN(rows[i][k])) {
          if (previous_was_number) {
            return {valid: false, error_number: 8, error: errors[8]};
          }
          sum_fields += parseInt(rows[i][k], 10);
          previous_was_number = true;
        } else {
          if (!/^[prnbqkPRNBQK]$/.test(rows[i][k])) {
            return {valid: false, error_number: 9, error: errors[9]};
          }
          sum_fields += 1;
          previous_was_number = false;
        }
      }
      if (sum_fields !== 8) {
        return {valid: false, error_number: 10, error: errors[10]};
      }
    }

    /* everything's okay! */
    return {valid: true, error_number: 0, error: errors[0]};
  }

 function validate_fen1() {
	fen =document.getElementById('fen').value;
    fen=fen.trim();
	document.getElementById('fen').value=fen;
	res =validate_fen(fen);
	return res;
 }

 function validate_fen2() {
	res=validate_fen1();
	if(res.valid==true)return true;
	fen =document.getElementById('fen');
	fen.style.backgroundColor = "red";
	document.getElementById("error_fen").className = 'nohidden';
	return false;	
 }

