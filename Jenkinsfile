pipeline 
{
    agent any 

    stages 
    {
        stage('Install dependencies') 
        {
            steps 
            {
                sh 'pacman -Syu'
                sh 'pacman -S gcc gtest cmake make sdl2'
            }
        }

        stage('Build') 
        {
            steps
            {
                sh 'mkdir build && cd build'
                sh 'cmake .. -DBUILD_WITH_TESTS=ON'
                sh 'make -j`nproc`'
            }
        }

        stage('Test')
        {
            steps
            {
                sh './test'
            }
        }

    }
}
