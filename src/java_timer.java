/*\
 *
 * Programa que calcula el tiempo que dura la ejecucion de un programa.
 *
 * Autores:
 *
 * Samantha Campisi (samanthac07@gmail.com)
 * Juan A. Escalante (jaescalante02@gmail.com)
\*/

import java.io.*;

public class java_timer {

  public static void main(String args[]) {

    String s, command = args[0];
    for(int i=1;i<args.length;i++){
    
      command+= " " + args[i];
    
    }

    try {

      long startTime, stopTime, elapsedTime;
      startTime = System.currentTimeMillis();
      Process p = Runtime.getRuntime().exec(command);

      p.waitFor();
      stopTime = System.currentTimeMillis();
      elapsedTime = stopTime - startTime;


      System.out.print("TIME: ");
      System.out.println(elapsedTime);
      System.exit(0);

    }
    catch (IOException e) {

      e.printStackTrace();
      System.exit(-1);
    }
    catch (InterruptedException e) {

      e.printStackTrace();
      System.exit(-1);
    }
  
  }

}
