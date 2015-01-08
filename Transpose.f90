!
! $compilateur -o Transpose -e95 Transpose.f90
! Walter Yvart
! Attention : si le nombre de colonnes n'est pas bon, une erreur de segmentation est produite
!

MODULE CALCUL
 IMPLICIT NONE
 INTEGER :: CAL_ERROR = 0
 CONTAINS
 
 SUBROUTINE TRANS(M,L)
  IMPLICIT NONE
  REAL(8), INTENT(In) :: M(:,:)
  REAL(8), INTENT(Out) :: L(:,:)
  IF( ( size(M,2).eq.size(L,1) ).AND.( size(M,1).eq.size(L,2) ) ) THEN
   L(:,:) = TRANSPOSE(M(:,:))
  ELSE
   CAL_ERROR = 1
   WRITE(*,*) "Transpose erreor : invalid size"
  END IF  
  CAL_ERROR=0
 END SUBROUTINE TRANS
 
END MODULE CALCUL

MODULE NSTDIO
 IMPLICIT NONE
 INTEGER, SAVE :: genID = 100
 INTEGER :: IO_ERROR
 CHARACTER(len=*), PARAMETER :: Fint3 = "(2X,I3, 2X)"
 CHARACTER(len=*), PARAMETER :: Sci10 = "(ES10.3,2X)"
 
 PUBLIC  :: WRITE_FILE
 PUBLIC  :: READ_FILE
 PRIVATE :: GETID
 PRIVATE :: GET_FILE
 PRIVATE :: GET_FILE_NF
 PRIVATE :: genID
 
 CONTAINS
 
  SUBROUTINE WRITE_FILE(nline, ncol, file, Matrix, header, firstcol)
  IMPLICIT NONE
  CHARACTER(len=255) :: file
  INTEGER, INTENT(In) :: nline, ncol,header, firstcol
  REAL(8), INTENT(In), DIMENSION(1:nline,1:ncol) :: Matrix
  INTEGER :: i,j,fl
  fl = GETID()
  OPEN(unit=fl, file=trim(file), status='UNKNOWN',action='WRITE')
  DO i=1,header
   WRITE(fl,*) "#"
  END DO
  DO i=1,nline
   IF(firstcol.gt.0) THEN
    WRITE(fl,Fint3, advance='NO') i
   END IF
   DO j=1,ncol
    WRITE(fl,Sci10,advance='NO') Matrix(i,j)
   END DO
   WRITE(fl,*)
  END DO
  CLOSE(fl)
 END SUBROUTINE WRITE_FILE
 
 SUBROUTINE READ_FILE(nline, ncol, file, Matrix, header, firstcol)
  IMPLICIT NONE
  CHARACTER(len=255) :: file
  INTEGER, INTENT(In) :: nline, ncol,header, firstcol
  REAL(8), INTENT(Out), DIMENSION(1:nline,1:ncol) :: Matrix
  IF(firstcol.ge.1) THEN
   CALL GET_FILE(nline, ncol, file, Matrix, header, firstcol)
  ELSE
   CALL GET_FILE_NF(nline, ncol, file, Matrix, header)
  END IF
 END SUBROUTINE READ_FILE

 ! PRIVATES
 
 INTEGER FUNCTION GETID()
  IMPLICIT NONE
  genID = genID + 1
  GETID = genID
  RETURN 
 END FUNCTION
 
 SUBROUTINE GET_FILE(nline, ncol, file, Matrix, header, firstcol)
  IMPLICIT NONE
  CHARACTER(len=255) :: file
  INTEGER, INTENT(In) :: nline, ncol,header, firstcol
  REAL(8), INTENT(Out), DIMENSION(1:nline,1:ncol) :: Matrix
  REAL(8), DIMENSION(1:ncol) :: L
  REAL(8), DIMENSION(1:firstcol) :: Id
  CHARACTER(len=2) :: dum
  INTEGER :: i,k,fl
  fl = GETID()
  OPEN(unit=fl, file=trim(file), status='OLD',action='READ')
  DO i=1,header
   READ(fl,*) dum
  END DO
  DO i=1,nline
   READ(fl,*) Id,L
   DO k=1,ncol
    Matrix(i,k) = L(k)
   END DO
  END DO
  CLOSE(fl)
 END SUBROUTINE GET_FILE

 SUBROUTINE GET_FILE_NF(nline, ncol, file, Matrix, header)
  IMPLICIT NONE
  CHARACTER(len=255) :: file
  INTEGER, INTENT(In) :: nline, ncol,header
  REAL(8), INTENT(Out), DIMENSION(1:nline,1:ncol) :: Matrix
  REAL(8), DIMENSION(1:ncol) :: L
  CHARACTER(len=2) :: dum
  INTEGER :: i,k,fl
  fl = GETID()
  OPEN(unit=fl, file=trim(file), status='OLD',action='READ')
  DO i=1,header
   READ(fl,*) dum
  END DO
  DO i=1,nline
   READ(fl,*) L
   DO k=1,ncol
    Matrix(i,k) = L(k)
   END DO
  END DO
  CLOSE(fl)
 END SUBROUTINE GET_FILE_NF

END MODULE NSTDIO

PROGRAM FILEOP
 USE CALCUL
 USE NSTDIO
 IMPLICIT NONE
 CHARACTER(len=255) :: line
 CHARACTER(len=255) :: name
 CHARACTER(len=255) :: Oname
 INTEGER :: nlines, ncols, nhead, nfirst, got, ioe, sc, vf
 REAL(8), DIMENSION(:,:), ALLOCATABLE :: M,L
 LOGICAL :: am, al
 am = .FALSE.
 al = .FALSE.
 ioe=1
 got=0
 DO WHILE((got.lt.4).AND.(ioe.ge.0)) 
  READ(5,'(A255)', iostat=ioe) line
  sc=SCAN(line,"!")
  vf=VERIFY(line,"! \t ")
  IF( (sc.gt.0).AND.( sc.lt.vf )) THEN
   CONTINUE
  ELSE
   IF(got.eq.0) THEN
    READ(line,'(A255)') name                !input name
    got=got+1
    !WRITE(*,*) "(",got,")Input:"//trim(name)
   ELSE IF(got.eq.1) THEN
    READ(line,'(A255)') Oname               !output name
    got=got+1
    !WRITE(*,*) "(",got,")Output:"//trim(Oname)
   ELSE IF(got.eq.2) THEN
    READ(line,*) nhead, nfirst              !nrows in head, ncols header
    got=got+1
    !WRITE(*,*) "(",got,")Header:", nhead, nfirst
   ELSE IF(got.eq.3) THEN
    READ(line,*) nlines, ncols              !DATA TO TRANSPOSE : nrows, ncols
    got=got+1
    !WRITE(*,*) "(",got,")Data:", nlines, ncols
   ELSE IF(got.ge.4) THEN
    EXIT
   ELSE
    CONTINUE
   END IF !end order commands
  END IF !end comment detect
 END DO
 
 IF( got.lt.4 ) THEN
  WRITE(*,*) "Err Main 0"
  STOP
 END IF
 IF( (nlines.ge.1).AND.(ncols.ge.1) ) THEN
  ALLOCATE(M(1:nlines, 1:ncols))
  ALLOCATE(L(1:ncols,1:nlines))
  am = .TRUE.
  al = .TRUE.
 ELSE
  WRITE(*,*) "Err Main 0"
  STOP
 END IF
 IF( (nhead.lt.0).OR.(nfirst.lt.0) ) THEN
  WRITE(*,*) "Err Main 1"
  STOP
 END IF
 CALL READ_FILE(nlines, ncols, name, M, nhead, nfirst)
 CALL TRANS(M,L)
 IF(CAL_ERROR.eq.0) THEN
 ELSE
  WRITE(*,*) "Err Main 3"
  STOP
 END IF
 CALL WRITE_FILE(size(L,1), size(L,2), Oname , L, nhead, nfirst)
 IF(am) THEN
  DEALLOCATE(M)
 END IF
 IF(al) THEN
  DEALLOCATE(L)
 END IF
 WRITE(*,*) "Complete"
END PROGRAM FILEOP

!
! README
!
! $> Prog < file
! 
! $>cat file
! input file name
! output file name
! nombre de lignes d'entete nombre de colonnes d'entete
! nombre de lignes nombre de colonnes (total moins nombres précédents)
!
