struct cyclic_buffor{
        char* bufforptr;
        char* readptr;
        char* writeptr;
        int size = 10;
};
cyclic_buffor* create_cyclic_buffor(){
    cyclic_buffor* buffor = new cyclic_buffor();
    buffor->bufforptr = new char[buffor->size];
    buffor->writeptr = buffor->bufforptr+1;
    buffor->readptr = buffor->writeptr;
    return buffor;
}
cyclic_buffor* create_cyclic_buffor(int i_size){
    cyclic_buffor* buffor = new cyclic_buffor();
    buffor->size = i_size;
    buffor->bufforptr = new char[buffor->size];
    buffor->writeptr = buffor->bufforptr+1;
    buffor->readptr = buffor->bufforptr;
    return buffor;
}
void delete_cyclic_buffor(cyclic_buffor* buff){
    delete(buff);
}
char read(cyclic_buffor* buff){
    char znak = *buff->readptr;
    if(buff->readptr+1 != buff->writeptr){
        if(buff->readptr == buff->bufforptr+buff->size-1 && buff->writeptr != buff->bufforptr){
            buff->readptr = buff->bufforptr;
        }
        else{
            buff->readptr++;
        }
    }
    return znak;
}
void write(cyclic_buffor* buff, char znak){
    *buff->writeptr = znak;
    if(buff->readptr != buff->writeptr+1){
        if(buff->writeptr == buff->bufforptr+buff->size-1 && buff->readptr != buff->bufforptr){
            buff->writeptr = buff->bufforptr;
        }
        else{
            buff->writeptr++;
        }
    }
}