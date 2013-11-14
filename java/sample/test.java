
import static kazmath.jkazmath.*;

import java.nio.FloatBuffer;
import java.text.DecimalFormat;

public class test {



    public static void main(String[] args) {
        test t=new test();
        t.run();
    }
    
    // do some splurious kazmath operations to show it in use...
    
    public void run() {
        // create a mat4 struct...
        FloatBuffer mat = kmCreateKmMat4();
        
        kmMat4Identity(mat);

        // vec3 structs
        FloatBuffer eye = kmCreateKmVec3();
        FloatBuffer centre = kmCreateKmVec3();
        FloatBuffer up = kmCreateKmVec3();

        
        kmVec3Fill(eye,    0, 0, 0);
        kmVec3Fill(centre, 0, 0,-5);
        kmVec3Fill(up,     0, 1, 0);

        
        kmMat4LookAt(mat, eye, centre, up);
        kmMat4RotationYawPitchRoll(mat, 0.1f,0.2f,0.3f);
        
        DecimalFormat df = new DecimalFormat("' '0.000;'-'0.000");
                
        // dump the end result
        System.out.println(df.format(mat.get( 0))+", "+df.format(mat.get( 1))+", "+df.format(mat.get( 2))+", "+df.format(mat.get( 3)));
        System.out.println(df.format(mat.get( 4))+", "+df.format(mat.get( 5))+", "+df.format(mat.get( 6))+", "+df.format(mat.get( 7)));
        System.out.println(df.format(mat.get( 8))+", "+df.format(mat.get( 9))+", "+df.format(mat.get(10))+", "+df.format(mat.get(11)));
        System.out.println(df.format(mat.get(12))+", "+df.format(mat.get(13))+", "+df.format(mat.get(14))+", "+df.format(mat.get(15)));
        
        FloatBuffer v1 = kmCreateKmVec3();
        FloatBuffer v2 = kmCreateKmVec3();
        
        kmVec3Fill(v1, 0, 0, 0);
        kmVec3Fill(v2, 1, 2, 3);
        
        kmVec3Lerp(v1, v1, v2, 0.5f);
        
        System.out.println("\n" + df.format(v1.get( 0))+", "+df.format(v1.get( 1))+", "+df.format(v1.get( 2)));
        
               
    }
    
}
