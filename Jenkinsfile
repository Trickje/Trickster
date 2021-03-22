pipeline {
    agent any
    triggers {
              pollSCM 'H/5 * * * *'
            }

    stages {
        stage('Sync'){
            steps{
                
                git credentialsId: '3d28b963-d3d4-40dd-be84-82232ee6a44b', url: 'https://github.com/Trickje/Trickster'
                
            }
            
        }
        stage('Build') {
            steps {
            //step([$class: 'GitHubSetCommitStatusBuilder'])
            //echo 'Changed the status to pending on GitHub'
            bat '"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/MSBuild/Current/Bin/amd64/msbuild.exe" "%WORKSPACE%/TricksterEngine.sln" /t:build /p:Configuration="VulkanDebug" /p:Platform="x64" /p:"verbosity=diagnostic"'
            }
        }
        stage('Test') {
            steps {
                echo 'Testing..'
            }
        }
        stage('Github Status') {
            steps {
                step([$class: 'GitHubCommitStatusSetter', reposSource: [$class: 'ManuallyEnteredRepositorySource', url: 'https://github.com/Trickje/Trickster']])
        //commitChangeset = sh(returnStdout: true, script: 'git diff-tree --no-commit-id --name-status -r HEAD').trim()
        //step([$class: 'GitHubCommitStatusSetter', statusResultSource: [$class: 'ConditionalStatusResultSource', results: []]])
                echo 'Deploying....'
            }
        }
        stage('Discord Notify'){
            steps{
                
             discordSend description: 'Jenkins Pipeline build', footer: getChangeString(), result: currentBuild.currentResult, title: 'Trickster Engine', thumbnail: 'https://pics.freeicons.io/uploads/icons/png/9320366201552037063-512.png', webhookURL: 'https://discordapp.com/api/webhooks/823671188445200405/paDZql7uR6_EYO4cXJHVhouJGvqn9eQeNtOutyw5p5zImnsSETPgXf4Sc-ToJQAYNXgT'
            }
            
        }
    }
    
    
}
@NonCPS
def getChangeString() {
     MAX_MSG_LEN = 100
     def changeString = ""
    
     echo "Gathering SCM changes"
     def changeLogSets = currentBuild.changeSets
                for (int i = 0; i < changeLogSets.size(); i++) {
                    def entries = changeLogSets[i].items
                    for (int j = 0; j < entries.length; j++) {
                        def entry = entries[j]
                        changeString += "[${entry.author}] on [${new Date(entry.timestamp).format('MM/dd HH:mm')}]: ${entry.msg} \n"
                       
                    }
                }
     if (!changeString) {
     changeString = " No new changes"
     }
 return changeString
}