(defun input()
    (write-line "no of proc : " )	
    (defvar proc (read) )
    (write-line "no of tasks : " )
    (defvar tasks (read) )
    (terpri)

  (defvar task_time (make-array '( 100 50)))
	(dotimes (i tasks)    ;reading tasks * proc
	 ( setq temp 0)   
	 (dotimes (j proc )
        (setf (aref task_time i j) (read) )
	      (setq temp (+ temp (aref task_time i j) ))
	 )

	 (setf (aref avg i) (/ temp proc ))
	)

  
  (defvar adj (make-array '(100 100)))


	(dotimes (i 100)  
	   (dotimes (j 100 )
             (setf (aref adj i j) -1 )
	   )
	)

  (setq edges_count (read) )       
	(dotimes (i edges_count)
	  (setq x (read) ) (setq y (read) ) (setq z (read) )
		(setf (aref adj x y ) z )
	)  	
) 

(defun rec_rank(i) 
  (setq maxi 0)
  (setq r_temp 0)

   ( dotimes (j tasks)
      (if(  /=  (aref adj i j) -1 )
         (progn
	         (setq r_temp (rec_rank j))
	         (if (< maxi ( + (aref adj i j) r_temp))
	           (setq maxi ( +  (aref adj i j)r_temp))
	         )
          )
          (setq maxi 0)
      )
    )

   (setf (aref rank i) (+ (aref avg i) maxi))
   (aref rank i)
)

(defun  findrank()      
  (dotimes(i tasks)
    (rec_rank i)
   )
)

(defun sorting() 
 (setq sort_temp (- tasks 1))
 (dotimes (i tasks)
  (dotimes (j sort_temp) 
    (if (<  (aref rank j) (aref rank (+ j 1)) )
	(progn 
	 (rotatef (aref rank j) (aref rank (+ j 1)) )
	 (rotatef (aref rank_order j) (aref rank_order (+ j 1)) )
	)
    )		
	
  )
 )
 ) 
    
(progn
  (defvar rank (make-array '(100)))
  (defvar avg (make-array '(100)))
  (defvar rank_order (make-array '(100)))

  (input)
 
  (dotimes (i 100)
   (setf (aref rank_order i) i )
  ) 

 (findrank)
 (sorting)	

	
  (dotimes (i tasks)
   (print (aref rank i) )
   (print (aref rank_order i)  )
  )

  


)







