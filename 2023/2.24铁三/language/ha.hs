import Data.Bits
import Data.Char (ord, chr)
import Control.Monad (unless)
import Data.Function (on)
import System.IO
  ( hGetContents )
import System.Process
  ( runInteractiveCommand
  , waitForProcess
  )
import System.Exit
  ( ExitCode ( .. ) )
f1 :: String -> String -> String
f1 = zipWith (fmap chr . xor `on` ord)
f2 :: String -> IO (String)
f2 command =
  do (_pIn, pOut, pErr, handle) <- runInteractiveCommand command
     exitCode <- waitForProcess handle
     output   <- hGetContents pOut
     return output
main = do
    contents <- readFile "flag.txt"
    --putStrLn "Key?"
    number <- getLine
    let num=read number :: Integer
    let arr=[13817382882945892163, 11981362477940068859, 15565376004970860571, 9289653146223670153, 10756880856242106767, 9894127981516380841, 10238229679407507287, 9710703753422507729, 10657889945311313641, 13504220130461522129, 13803825974548049497, 14920116056534898233, 15632599539576708509, 13759455315204814571, 10093307776393200077, 15238687618512734183]
    let t = map (mod num) arr
    --putStrLn $ show t
    unless (num < (product arr))
        $ error "too big"
    unless (t==[10310396961361403274, 4961986008647557365, 13968825266080359119, 5116430780059239731, 7325364491754059752, 4473328037653903247, 2810438785866611784, 8321467780510747289, 5892005478714175254, 12851142119012897040, 7300192969480689261, 2282831960924441046, 5121069978296721861, 6987308129012337675, 7813058204868331779, 11203522277396859834])
        $ error "assertion failed"
    --putStrLn $ show t
    let z="echo -n " ++ show num ++ " | openssl dgst -binary -sha256 | base64"
    --putStrLn z
    let a=f2 $ z
    aa <- a
    --putStrLn aa
    let b=length contents
    putStr $ f1 contents $ take b aa
    --putStrLn $ init contents